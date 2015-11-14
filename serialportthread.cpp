
#include "serialportthread.h"

#include <QtWidgets>
#include <QTime>
#include "command.h"

QT_USE_NAMESPACE

command_value_t EndTransaction = {"", ""};

SerialPortThread::SerialPortThread(QObject *parent)
    : QThread(parent)
{
    serialPortChange = true;
    start();
}


SerialPortThread::~SerialPortThread()
{
    {
        QMutexLocker locker(&mutex);
        while (!transactions.isEmpty()) {
            transactions.dequeue();
        }
        transactions.enqueue(EndTransaction);
        transactionAdded.wakeOne();  
    }
    wait();
    qDebug() << "SerialPortThread::~SerialPortThread()";
}
void SerialPortThread::removeAllTransactions()
{
    QMutexLocker locker(&mutex);
    while (!transactions.isEmpty()) {
        transactions.dequeue();
    }
}

void SerialPortThread::run()
{
    command_value_t transact;
    QSerialPort serialPort;
    while (1) {
        mutex.lock();
        if (transactions.isEmpty())
            transactionAdded.wait(&mutex);
        transact = transactions.dequeue();
        if (transact.command.isEmpty()) break;
        mutex.unlock();

        updateSerialPort(serialPort);

        serialPortSend(transact, serialPort);
    }
}
void SerialPortThread::updateSerialPort(QSerialPort &serialPort)
{
    QMutexLocker locker(&mutex);
    if (serialPortChange) {
        serialPortChange = false;
       if (!setSerialPort(serialPort)) {
            qDebug() << "Failed to Open \"COM PORT\" !"
                     << serialPort.errorString();
        } else {
            qDebug() << "Open \"COM PORT\" successed!";
       }
    }
}
void SerialPortThread::serialPortChanged(QString /*str*/)
{
    qDebug() << "void SerialPortThread::serialPortChanged";
    serialPortChange = true;
}
void SerialPortThread::addTransaction(const command_value_t cv)
{
    if (cv.value.isEmpty()) {
        qDebug() << cv.command << "command is not in the table, please check!";
        return;   // 命令出错时容错处理 FIXME: <容错不应出现在此>
    }
    QMutexLocker locker(&mutex);
    for (int i = 0; i < transactions.size(); ++i) {
        if (transactions.at(i).value == cv.value) {
            transactionAdded.wakeOne();
            return;
        }
    }
    transactions.enqueue(cv);
    transactionAdded.wakeOne();
}
/**
 * @brief SerialPortThread::serialPortSend
 * @param cv
 * @param serialPort
 */
void SerialPortThread::serialPortSend(command_value_t &cv, QSerialPort &serialPort)
{

    static int crcErrorTimes;
tryagain:
    //while (!serialPort.clear(QSerialPort::AllDirections));
    serialPort.write(cv.value); // write request
    if (serialPort.waitForBytesWritten(500)) { // wait written
        if (serialPort.waitForReadyRead(1000)) {// read response
            QByteArray responseData = serialPort.readAll();
            while (serialPort.waitForReadyRead(10)) {
                responseData += serialPort.readAll();
            }
//            qDebug() << "responseData = " << responseData;

            if (responseData.left(2) != cv.value.left(2)) {
                goto tryagain; //FIXME:
            }
            if (responseData.at(1) == SREAD) {
                if (responseData.size() != 7)  goto tryagain; //FIXME:
            }
            if ((responseData.at(1) == SWRITE)) {
                if ( responseData.size() != 8)  goto tryagain; //FIXME:
            }
            quint8 recLength = responseData.size() - 2;
            if (GetCRC16(responseData.left(recLength)) == responseData.right(2)) {
                if (responseData.at(1) == 0x03) {
                    cv.value = responseData.left(recLength).remove(0, 3);
                    emit response(cv);
                }
            } else {
                if (tryToRecover()) goto tryagain;
                qDebug() << "responseData = " << responseData << QTime::currentTime();
                crcErrorTimes++;
                emit error(QString("crcerror: %1, times = %2")
                           .arg(QTime::currentTime().toString())
                           .arg(QString::number(crcErrorTimes)));
            }
        } else {
            if (tryToRecover()) goto tryagain;
            qDebug() << "read time out";
            emit error(QString("Read Error: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        }
    } else {
        if (tryToRecover()) goto tryagain;
        emit error(QString("Write Error: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }
}
bool SerialPortThread::tryToRecover()
{
    static int times = 0;
    if (times < TRYTIMES) {
        times++;
        return true;
    } else {
        times = 0;
        return false;
    }
}

bool SerialPortThread::setSerialPort(QSerialPort &serial)
{
    QSettings settings("DoLong Inc.", "CRS control system");
    QString comname = settings.value("s1name").toString();
    QString baudrate = settings.value("s1baudrate").toString();
    QString parity = settings.value("s1parity").toString();
    QString stopbits = settings.value("s1stop").toString();
    QString flowcontrol = settings.value("s1flowcontrol").toString();
    QString databits = settings.value("s1databits").toString();

    serial.close();
    serial.setPortName(comname);
    serial.setBaudRate(getBaudRate(baudrate));
    serial.setParity(getParity(parity));
    serial.setStopBits(getStopBits(stopbits));
    serial.setFlowControl(getFlowControl(flowcontrol));
    serial.setDataBits(getDataBits(databits));
    return serial.open(QIODevice::ReadWrite);
}

QSerialPort::BaudRate SerialPortThread::getBaudRate(QString str)
{
    return (str == "Baud1200") ? QSerialPort::Baud1200 :
            (str == "Baud2400") ? QSerialPort::Baud2400 :
            (str == "Baud4800") ? QSerialPort::Baud4800 :
            (str == "Baud9600") ? QSerialPort::Baud9600 :
            (str == "Baud19200") ? QSerialPort::Baud19200 :
            (str == "Baud38400") ? QSerialPort::Baud38400 :
            (str == "Baud57600") ? QSerialPort::Baud57600 :
            (str == "Baud115200") ? QSerialPort::Baud115200 : QSerialPort::UnknownBaud;
}
QSerialPort::DataBits SerialPortThread::getDataBits(QString str)
{
   return (str == "Data5") ? QSerialPort::Data5 :
        (str == "Data6") ? QSerialPort::Data6 :
        (str == "Data7") ? QSerialPort::Data7 :
        (str == "Data8") ? QSerialPort::Data8 : QSerialPort::UnknownDataBits;
}

QSerialPort::Parity SerialPortThread::getParity(QString str)
{
    return (str == "NoParity") ? QSerialPort::NoParity :
            (str == "EvenParity") ? QSerialPort::EvenParity :
            (str == "OddParity") ? QSerialPort::OddParity :
            (str == "SpaceParity") ? QSerialPort::SpaceParity :
            (str == "MarkParity") ? QSerialPort::MarkParity : QSerialPort::UnknownParity;
}

QSerialPort::FlowControl SerialPortThread::getFlowControl(QString str)
{
    return (str == "NoFlowControl") ? QSerialPort::NoFlowControl :
            (str == "HardwareControl") ? QSerialPort::HardwareControl :
            (str == "SoftwareControl") ? QSerialPort::SoftwareControl : QSerialPort::UnknownFlowControl;
}

QSerialPort::StopBits SerialPortThread::getStopBits(QString str)
{
    return (str == "OneStop") ? QSerialPort::OneStop :
            (str == "OneAndHalfStop") ? QSerialPort::OneAndHalfStop :
            (str == "TwoStop") ? QSerialPort::TwoStop : QSerialPort::UnknownStopBits;
}
/**
 * @brief GetRevCrc_16
 * @param ptable
 * @param pData
 * @param nLength
 * @param init
 * @return 返回值为校验结果
 */
QByteArray SerialPortThread::GetCRC16(QByteArray data,
                                       const quint16 *ptable,
                                       quint16 init)
{
    unsigned short cRc_16 = init;
    unsigned char temp;
    for (int i = 0; i < data.size(); ++i)
    {
      temp = cRc_16 & 0xFF;
      cRc_16 = (cRc_16 >> 8) ^ ptable[(temp ^ data.at(i)) & 0xFF];
    }
    QByteArray byteArray;
    byteArray.append(quint8(cRc_16)).append(quint8(cRc_16 >> 8));
    return byteArray;
}

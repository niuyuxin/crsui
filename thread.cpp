#include <QtCore>
#include <iostream>
#include <QMutexLocker>

#include "thread.h"
#include "command.h"

Thread::Thread(QObject *parent)
    :QThread(parent)
{
    pressureKeepTimes = 0;
    stopped = false;
    oilpumpStart = WAIT;
    status = WAIT;
    testItemVector.clear();
}
Thread::~Thread()
{
    {
        QMutexLocker locker(&threadMutex);
        stopped = true;
        waitCondition.wakeOne();
    }
    wait();
    qDebug() << "Thread::~Thread()";
}

void Thread::run()
{
    forever {
        {//exit
            QMutexLocker locker(&threadMutex);
            if (stopped) {
                stopped = false;
                status = WAIT;
                qDebug() << "Thread::run() exit";
                break;
            }
        }
        testProcess();
        oilpumpProcess();
        getAnalogData();
        msleep(TICKS);
    }
}

void Thread::testProcess()
{
    static QStringList currentInfo;
    int actValue = 0;
    QString stringValue;
    int setValue = 0;
    int error = 0;
    int stableTimes = 0;

    QMutexLocker locker(&threadMutex);
    switch (status) {
    case WORKCONDITION: // 测试工况采集及检测测试结果
        qDebug() << "status == WORKCONDITION";
        if (testItemVector.isEmpty()) {
            status = WAIT;
            finishedTest("error");
        } else {
            currentInfo = testItemVector.first();
            QString speed = currentInfo.at(1);
            QString speedNum = currentInfo.at(6);
            int speedValue = speed.toInt() + ((speedNum.toInt()+1) * 4096);
            emit request(Command::requestToByteArray("Speed", QString::number(speedValue)));
            emit request(Command::requestToByteArray("InjectorCountSet", currentInfo.at(2)));
            emit request(Command::requestToByteArray("InjectorControlTime", currentInfo.at(3)));
            emit request(Command::requestToByteArray("RailPressureSetValue", currentInfo.at(4)));
            emit request(Command::requestToByteArray("InjectorFre", currentInfo.at(5)));

            emit currentWorkingCondition(currentInfo);
            testItemVector.removeFirst();
            emit request(Command::requestToByteArray("SystemControlInfo"));
            status = WAITPRESSURE;
        }
        break;
    case WAITPRESSURE:
        actValue = Command::getCommand("RailPressure")->paraValue.toDouble();
        stringValue = currentInfo.at(4);
        setValue = stringValue.toInt();
        error = Command::getCommand("RailPressureError")->paraValue.toUInt();
        stableTimes = Command::getCommand("RPStableTimes")->paraValue.toUInt();
        if (((actValue + error) >= setValue) && (actValue <= (setValue + error))) {//pressure == sl.at(1)) {
            pressureKeepTimes += TICKS;
            if (pressureKeepTimes >=  stableTimes) {
                pressureKeepTimes = 0;
                qDebug() << "status = WAITPRESSURE" << stableTimes << error;
                status = WAITCOUNT;
                stringValue = currentInfo.at(0);
                if (stringValue.contains("-R")) {
                    Command::updateCommandValue("FuelSupplyEMV", "0");
                    Command::updateCommandValue("FuelReturnEMV", "1");
                } else {
                    Command::updateCommandValue("FuelSupplyEMV", "1");
                    Command::updateCommandValue("FuelReturnEMV", "0");
                }
                Command::updateCommandValue("InjectorCountSwitch", "1");
                Command::updateSystemControlInfo();
                emit request(Command::requestToByteArray("SystemControlInfo"));
                Command::getCommand("InjectorCountValue")->paraValue = 0;
            }
        } else {
            pressureKeepTimes = 0;
        }
        break;
    case WAITCOUNT:
        actValue = Command::getCommand("InjectorCountValue")->paraValue.toInt();

        emit updatePlot(actValue, Command::getCommand("InjFlowValue")->paraValue.toFloat());
        stringValue = currentInfo.at(2);
        setValue = stringValue.toInt();
        if (actValue >= setValue) {
            qDebug() << "status = WAITCOUNT";
            if (testItemVector.isEmpty()) {
                status = WAIT;
                emit finishedTest("testEnd");
                qDebug() << "system StopRunning";
            } else {
                Command::updateCommandValue("InjectorCountSwitch", "0");
                Command::updateSystemControlInfo();
                emit request(Command::requestToByteArray("SystemControlInfo"));
                status = WORKCONDITION;
            }
        } else {
            emit request(Command::requestToByteArray("InjectorCountValue"));
        }
        break;
    case WAITSTOP:
        status = WAIT;
        emit request(Command::requestToByteArray("SystemControlInfo"));
        qDebug() << "user StopRunning";
        break;
    default:
        pressureKeepTimes = 0;
        break;
    }
}

void Thread::response(const command_value_t cv)
{
    QMutexLocker locker(&threadMutex);
    QString value = responseToString(cv);
    Command::updateCommandValue(cv.command, value);
    emit updateValue(cv.command, value);
}
QString Thread::responseToString(const command_value_t &cv)
{
    QString stringValue;
    quint16 value = cv.value.at(0);
    quint8 lowByte = cv.value.at(1);
    value = (value << 8) + lowByte;

    qint16 coe = Command::getCommand(cv.command)->paraCoe;
    if (coe != 1) {
        double temp = double(value)/coe;
        stringValue = QString::number(temp, 'f', 1);
    } else {
        stringValue = QString::number(value);
    }

    return stringValue;
}
int Thread::getStablePressure(int pressure)
{
    static quint32 allPressure;
    if (pressureBuffer.size() > 1000/TICKS) {
        allPressure -= pressureBuffer.first();
        pressureBuffer.removeFirst();
    }
    pressureBuffer.append(pressure);
    allPressure += pressure;
    return allPressure * TICKS / 1000;
}
void Thread::timeout(const QString &str)
{
    QMutexLocker locker(&threadMutex);
    qDebug() << str << "timeout";
    responseValue = "error";
    waitCondition.wakeOne();
}

void Thread::getTestItems(QVector <QStringList> &vsl)
{
    QMutexLocker locker(&threadMutex);
    testItemVector = vsl;
    for (int i = 0; i < testItemVector.size(); ++i) {
        qDebug() << testItemVector.at(i);
    }
    this->status = WORKCONDITION;
}
void Thread::oilpumpButtonClicked(bool clicked)
{
    QMutexLocker locker(&threadMutex);
    oilpumpStart = clicked ? WORKCONDITION : STOPCONDITION;
    if (oilpumpStart == WORKCONDITION) {
        Command::updateCommandValue("OilPumpMoto", "1");
    } else {
        Command::updateCommandValue("OilPumpMoto", "0");
    }
    Command::updateSystemControlInfo();
    qDebug() << "oilpumpStart = " << oilpumpStart;
    //waitCondition.wait(&threadMutex);
}
void Thread::oilpumpProcess()
{
    QMutexLocker locker(&threadMutex);
    if (oilpumpStart == WORKCONDITION) {
        emit request(Command::requestToByteArray("SystemControlInfo"));
        oilpumpStart = WAIT;
    } else if (oilpumpStart == STOPCONDITION) {
        emit request(Command::requestToByteArray("SystemControlInfo"));
        oilpumpStart = WAIT;
    }
}
void Thread::getAnalogData()
{
//    QMutexLocker locker(&threadMutex);
    static quint32 times = 0;
    times++;
    if (times % (FLOWGAP/TICKS) == 0) {
        emit request(Command::requestToByteArray("InjFlowPulseNum"));
    }
    if (times % (PRESSUREGAP/TICKS) == 0) {
        emit request(Command::requestToByteArray("RailPressure"));
    }
    if (times % (TEMPERATUREGAP/TICKS) == 0) {
        emit request(Command::requestToByteArray("TemperatureValue"));
    }
    if (times % (SPEEDGAP/TICKS) == 0) {
        emit request(Command::requestToByteArray("SpeedValue"));
    }

}

void Thread::stop()
{
    {
        QMutexLocker locker(&threadMutex);
        stopped = true;
        waitCondition.wakeOne();
        //serialPortThread->removeAllTransactions();
        qDebug() << "Thread::stop";
    }
    wait();
}
void Thread::runTest()
{
    QMutexLocker locker(&threadMutex);
    this->status = WORKCONDITION;
}
void Thread::stopTest()
{
    QMutexLocker locker(&threadMutex);
    this->status = WAITSTOP;
}



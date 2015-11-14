#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QQueue>
#include "usertype.h"
#include "command.h"

class SerialPortThread : public QThread
{
    Q_OBJECT
public:
    SerialPortThread(QObject *parent = 0);
    ~SerialPortThread();
    void removeAllTransactions();
    void run();
signals:
    void response(command_value_t cv);
    void error(QString);
public slots:
    void addTransaction(const command_value_t vc);
    bool setSerialPort(QSerialPort &);
    void serialPortChanged(QString);
private:
    enum {TRYTIMES = 1};
    bool tryToRecover();
    void updateSerialPort(QSerialPort &serialPort);
    void serialPortSend(command_value_t &cv, QSerialPort &);
    QByteArray GetCRC16(QByteArray data, const quint16 *ptable = Command::crcTable, quint16 init = 0xffff);
    QSerialPort::BaudRate getBaudRate(QString string);
    QSerialPort::DataBits getDataBits(QString str);
    QSerialPort::Parity getParity(QString str);
    QSerialPort::FlowControl getFlowControl(QString str);
    QSerialPort::StopBits getStopBits(QString str);
    volatile bool serialPortChange;
    QMutex mutex;
    QQueue <command_value_t> transactions;
    QWaitCondition transactionAdded;
};

#endif // SERIALPORT_H


#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include "serialportthread.h"

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(QObject *parent = 0);
    ~Thread();
    void stop();
    void runTest();
    void stopTest();
protected:
    void run();
private slots:
    void response(const command_value_t str);
    void timeout(const QString &);
    void getTestItems(QVector <QStringList> &vsl);
    void oilpumpButtonClicked(bool);
signals:
    void request(const command_value_t str);
    void currentWorkingCondition(QStringList str);
    void finishedTest(const QString &error);
    void updateValue(QString item, QString value);
    void exitRun();
    void updatePlot(int , float);
private:
    enum {TICKS = 50, WORKCONDITION = 1, WAITPRESSURE = 2, WAITCOUNT = 3, STOPCONDITION = 4, WAITSTOP = 0xFE, WAIT = 0xFF};
    enum {FLOWGAP = 500, COUNTSGAP = 1, PRESSUREGAP = 300, TEMPERATUREGAP = 1000, SPEEDGAP = 200};
    volatile bool stopped;
    QString responseToString(const command_value_t &);
    void testProcess();
    void oilpumpProcess();
    void getAnalogData();
    int getStablePressure(int pressure);
    QMutex threadMutex;
    QWaitCondition waitCondition;
    quint8 status;
    volatile bool machineIsRunning;
    quint8 oilpumpStart;
    quint16 pressureKeepTimes;
    QString responseValue;
    QList <int> pressureBuffer;
    QVector <QStringList> testItemVector;
    SerialPortThread *serialPortThread;
};


#endif

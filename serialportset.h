#ifndef SERIALPORTSET_H
#define SERIALPORTSET_H

#include <QtWidgets>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include "ui_serialportdialog.h"

class SerialPortSet : public QDialog, public Ui::serialPortDialog
{
    Q_OBJECT
public:
    SerialPortSet(QWidget *p = 0);
    ~SerialPortSet();
signals:
    void serialPortChanged(QString str);
private slots:
    void on_okButton_clicked();
private:

    QSerialPortInfo serialPortInfo;
    void readSerialPortSetting();
    void writeSerialPortSetting();
public:
};


#endif // SERIALPORTSET_H


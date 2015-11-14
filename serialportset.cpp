/**
 * ========================================================================
 * filename: serialportset.cpp
 * author: niuxin
 * company: hand
 * data: 2015/09/29
 * brief:
 * 1, 没有实现各个串口之见选择的互斥操作
 * 2, 简单的定义 cri 为串口1
 *             piezo 串口 2
 *             油泵   串口3
 *             串口4 预留
 *            当ok键按下时， 更新串口1的数据， 其他不变， 留作以后扩展
 *                                                    2015/09/29 niuyuxin
 * ========================================================================
 */
#include "serialportset.h"

SerialPortSet::SerialPortSet(QWidget *p)
    :QDialog(p)
{
    setupUi(this);
    piezoGroupBox->hide();
    pumpGroupBox->hide();
    otherGroupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    QList<QSerialPortInfo> serialPortInfoList = serialPortInfo.availablePorts();
    QStringList serialPortName;
    for (int i = 0; i < serialPortInfoList.size(); ++i) {
        serialPortName.append(serialPortInfoList.at(i).portName());
    }
    criSerialPortBox->addItems(serialPortName);
    pumpSerialPortBox->addItems(serialPortName);
    piezoSerialPortBox->addItems(serialPortName);
    otherSerialPortBox->addItems(serialPortName);

    QSettings settings("DoLong Inc.", "CRS control system");
    if (!settings.value("configured", false).toBool()) {
        settings.setValue("configured", true);
        writeSerialPortSetting();
        qDebug() << "writeSerialPortSetting();";
    } else {
        readSerialPortSetting();
        qDebug() << "readSerialPortSetting();";
    }
}
SerialPortSet::~SerialPortSet()
{
}

void SerialPortSet::on_okButton_clicked()
{
    writeSerialPortSetting();
    emit serialPortChanged("xxx");
    close();
}
void SerialPortSet::writeSerialPortSetting()
{
    QSettings settings("DoLong Inc.", "CRS control system");

    settings.setValue("s1name", criSerialPortBox->currentText());
    settings.setValue("s1baudrate", criBaudBox->currentText());
    settings.setValue("s1parity", criParityBox->currentText());
    settings.setValue("s1stop", criStopBox->currentText());
    settings.setValue("s1flowcontrol", criFlowBox->currentText());
    settings.setValue("s1databits", criDataBitsBox->currentText());

    settings.setValue("s2name", piezoSerialPortBox->currentText());
    settings.setValue("s2baudrate", piezoBaudBox->currentText());
    settings.setValue("s2parity", piezoParityBox->currentText());
    settings.setValue("s2stop", piezoStopBox->currentText());
    settings.setValue("s2flowcontrol", piezoFlowBox->currentText());
    settings.setValue("s2databits", piezoDataBitsBox->currentText());

    settings.setValue("s3name", pumpSerialPortBox->currentText());
    settings.setValue("s3baudrate", pumpBaudBox->currentText());
    settings.setValue("s3parity", pumpParityBox->currentText());
    settings.setValue("s3stop", pumpStopBox->currentText());
    settings.setValue("s3flowcontrol", pumpFlowBox->currentText());
    settings.setValue("s3databits", pumpDataBitsBox->currentText());

    settings.setValue("s4name", otherSerialPortBox->currentText());
    settings.setValue("s4baudrate", otherBaudBox->currentText());
    settings.setValue("s4parity", otherParityBox->currentText());
    settings.setValue("s4stop", otherStopBox->currentText());
    settings.setValue("s4flowcontrol", otherFlowBox->currentText());
    settings.setValue("s4databits", otherDataBitsBox->currentText());
}
void SerialPortSet::readSerialPortSetting()
{
    QSettings settings("DoLong Inc.", "CRS control system");
    criSerialPortBox->setCurrentText(settings.value("s1name").toString());
    criBaudBox->setCurrentText(settings.value("s1baudrate").toString());
    criParityBox->setCurrentText(settings.value("s1parity").toString());
    criStopBox->setCurrentText(settings.value("s1stop").toString());
    criFlowBox->setCurrentText(settings.value("s1flowcontrol").toString());
    criDataBitsBox->setCurrentText(settings.value("s1databits").toString());

    piezoSerialPortBox->setCurrentText(settings.value("s2name").toString());
    piezoBaudBox->setCurrentText(settings.value("s2baudrate").toString());
    piezoParityBox->setCurrentText(settings.value("s2parity").toString());
    piezoStopBox->setCurrentText(settings.value("s2stop").toString());
    piezoFlowBox->setCurrentText(settings.value("s2flowcontrol").toString());
    piezoDataBitsBox->setCurrentText(settings.value("s2databits").toString());

    pumpSerialPortBox->setCurrentText(settings.value("s3name").toString());
    pumpBaudBox->setCurrentText(settings.value("s3baudrate").toString());
    pumpParityBox->setCurrentText(settings.value("s3parity").toString());
    pumpStopBox->setCurrentText(settings.value("s3stop").toString());
    pumpFlowBox->setCurrentText(settings.value("s3flowcontrol").toString());
    pumpDataBitsBox->setCurrentText(settings.value("s3databits").toString());

    otherSerialPortBox->setCurrentText(settings.value("s4name").toString());
    otherBaudBox->setCurrentText(settings.value("s4baudrate").toString());
    otherParityBox->setCurrentText(settings.value("s4parity").toString());
    otherStopBox->setCurrentText(settings.value("s4stop").toString());
    otherFlowBox->setCurrentText(settings.value("s4flowcontrol").toString());
    otherDataBitsBox->setCurrentText(settings.value("s4databits").toString());

}

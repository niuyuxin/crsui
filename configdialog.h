#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H
#include <QWidget>
#include <QDebug>
#include "ui_configdialog.h"

class ConfigDialog : public QDialog, public Ui::configDialog
{
    Q_OBJECT
public:
    ConfigDialog(QWidget *p = 0);
    ConfigDialog(QWidget *p, QStringList sl);
public slots:
    void on_tableWidget_itemChanged(QTableWidgetItem *);
    void accept();
signals:
private:
    int defaultValue(QString first, QString last);

};

#endif // CONFIGDIALOG_H


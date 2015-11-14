#ifndef PARAMETERWIN_H
#define PARAMETERWIN_H

#include <QtWidgets>
#include "ui_parameterwin.h"
#include "cell.h"
#include "serialportthread.h"


class ParameterWin : public QDialog, public Ui::parameterWin
{
    Q_OBJECT

public:
    ParameterWin(QWidget *parent = 0);
private:
    //void CreatTableItem(TableInfo &ti, quint16 a);
    void creatTableItem(command_attr_t *command, quint16 size);
    void writeSettings();
//    bool readFile(const QString &fileName, Para_t *buffer, int elementnum);
//    bool writeFile(TableInfo &ti);
protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
signals:
    void sendData(command_value_t);
public slots:
    void somethingChanged(QTableWidgetItem *);
    void flushData();
    void itemCheck();
    void response(const QString &str);
    void timeout(const QString &str);
public:
    enum {MagicNumber = 0x12345678, TableNumber = 6, GROUPMAX = 50, FLUSHTIME = 1000};
private:
    QVector <QTableWidget *> tableWidgetVector;
    QTimer timer;
};

#endif // PARAMETERWIN_H

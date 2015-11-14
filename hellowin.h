#ifndef HELLOWIN_H
#define HELLOWIN_H

#include "ui_hellowin.h"
#include "parameterwin.h"
#include "langdialog.h"
#include "detailform.h"
#include "dataload.h"
#include "serialportset.h"
#include "serialportthread.h"

class BeginWin;
class DetailForm;

class HelloWin: public QWidget, public Ui::firstWidget
{
    Q_OBJECT
public:
    HelloWin(QWidget *p = 0);

    void widgetPrevNext(QWidget *prev = 0, QWidget *next = 0);
signals:
    void sendData(command_value_t);
    void treeWidgetDoubleClicked(QTreeWidgetItem *, int);
    void treeWidgetClicked(QTreeWidgetItem *, int);
    void showBackButton(bool);
    void showContinueButton(bool);
    void request(QString str);
    void serialPortChanged(QString str);
private slots:
    void on_beginTreeWidget_itemDoubleClicked(QTreeWidgetItem*,int);
    void on_beginTreeWidget_itemClicked(QTreeWidgetItem*, int);
    void on_continueButton_clicked();
    void on_backButton_clicked();
    void on_connectButton_clicked();
    void response(const QString &);
    void timeout(const QString &);
    void systemTime();
protected:
    bool event(QEvent *);
private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    QWidget *nextWidget;
    QWidget *previousWidget;
    LangDialog * langDialog;
    DataLoad * dataLoad;
    SerialPortSet *serialPortSet;
};


#endif // HELLOWIN_H


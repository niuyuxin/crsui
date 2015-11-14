#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDateTime>
#include <QTimer>
#include <QMainWindow>
#include "langdialog.h"
#include "parameterwin.h"
#include "helpwin.h"
#include "hellowin.h"
#include "detailform.h"
#include "datebaseform.h"
#include "testform.h"
#include "serialportthread.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
signals:

private slots:

protected:
    void keyPressEvent(QKeyEvent *);
private:
    HelpWin * helpWin;
    HelloWin *helloWin;
    DetailForm *detailForm;
    DateBaseForm *dateBaseForm;
    TestForm * testForm;
    ParameterWin *parameterWin;
    SerialPortThread *serialPortThread;
    QTimer systemTimer;
};

#endif // MAINWINDOW_H

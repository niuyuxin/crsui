#ifndef TESTFORM_H
#define TESTFORM_H

#include <QtWidgets>
#include "ui_testform.h"
#include "plotter.h"
#include "serialportthread.h"
#include "thread.h"
#include "configdialog.h"

class TestForm : public QWidget, Ui::testForm
{
    Q_OBJECT
public:
    TestForm(QWidget *parent = 0);
    ~TestForm();
    Thread *thread;
signals:
    void sendData(command_value_t);
    void testItems(QVector <QStringList> &);
public slots:
    void systemTime();
    void on_backButton_clicked();
    void on_continueButton_clicked();
    void on_tableWidget_cellClicked(int , int);
    void widgetPrevNext(QWidget *prev, QWidget *next);
    void receiveDataFromItemDetail(QVector<QStringList> &slv);
    void on_runButton_toggled(bool );
    void response(const QString &str);
    void timeout(const QString &str);
    void error(const QString &str);
    void updateValue(QString, QString);
    void on_configButton_clicked();
    void on_tempSetLineEdit_returnPressed();
    void on_tempSlider_valueChanged(int value);
    void on_tableWidget_itemChanged(QTableWidgetItem * item);
    void currentWorkingCondition(QStringList strList);
    void updatePlot(int, float);
protected:
    bool event(QEvent *);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private:
    void creatTable(QVector<QStringList> & stringListVector, QStringList &stringList);
    void setText(int row, int column, const QString &text);
    void resizeEvent(QResizeEvent *);
    QWidget *previousWidget;
    QWidget *nextWidget;
    Plotter *plotter;
//    QTimer *temperatureTimer;
//    QTimer *countTimer;
    QString partNumber;

    ConfigDialog *configDialog;
    PlotSettings plotSettings;
    QVector<QPointF> plotPoints;
    QStringList currentTestItem;
};



#endif // TESTFORM_H


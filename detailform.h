#ifndef DETAILFORM_H
#define DETAILFORM_H

#include <QtWidgets>
#include "ui_detailform.h"


class BeginWin;

class DetailForm : public QWidget, public Ui::detailForm
{
    Q_OBJECT
public:
    DetailForm(QWidget *p = 0);
    void widgetPrevNext(QWidget *prev = 0, QWidget *next = 0);
signals:
    void showBackButton(bool status);
    void showContinueButton(bool status);
    void showLoadButton(bool status);
    void showDateBase();
    void sendDetailItemData(QVector<QStringList> &itemDetail);
public slots:
    void systemTime();
    void on_backButton_clicked();
    void on_continueButton_clicked();
    void on_loadButton_clicked();
    void receiveDataFromDataBase(QList <QTableWidgetItem *> * /* twil */);
private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void resizeEvent(QResizeEvent *);
    void collectItemData();
    void setText(int row, int column, const QString &text);
    QStringList getControlFeature(const QString &fileName, const QString &item);
    QVector <QStringList> getOperatingMode(const QString &fileName, const QString &item);
    void creatTable(QVector<QStringList> &stringListVector, QStringList &sl);
    bool dataBaseOk;
    QVector<QStringList> itemDetail;
    QWidget *previousWidget;
    QWidget *nextWidget;
};

#endif // DETAILFORM_H


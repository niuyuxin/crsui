#ifndef DATEBASEFORM_H
#define DATEBASEFORM_H

#include <QtWidgets>
#include "ui_datebaseform.h"

class DateBaseForm : public QWidget, public Ui::dateBaseForm
{
    Q_OBJECT
public:
    DateBaseForm(QWidget *p = 0);
    void widgetPrevNext(QWidget *prev = 0, QWidget *next = 0);
    void connectDB();
    void queryDB();
signals:
    void showBackButton(bool status);
    void showContinueButton(bool status);
    void showDateBaseButton(bool status);
    void dateBaseSelect(QList <QTableWidgetItem *> * /* twil */);
private slots:
    void systemTime();
    void on_backButton_clicked();
    void on_continueButton_clicked();
    void on_tableWidget_itemSelectionChanged();
    void on_searchButton_clicked();
    void on_partnumberLineEdit_textChanged(const QString &str);
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void resizeEvent(QResizeEvent *);
    bool readFile(const QString &filename);
    void creatTable(int row, int column, QStringList &sl);
    void setText(int row, int column, const QString &text);
    void search(const QString &str);
    QString dataBaseFileName;
    QWidget *previousWidget;
    QWidget *nextWidget;
};

#endif // DateBaseForm_H


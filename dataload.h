#ifndef DATALOAD_H
#define DATALOAD_H

#include <QtWidgets>

#include "ui_dataload.h"


class DataLoad : public QDialog, public Ui::dataDialog
{
    Q_OBJECT
public:
    DataLoad(QWidget *p = 0);
private:
    bool writeFile(const QString &fileName, const QString &tablename);
public slots:
    void on_openButton_clicked();
    void on_loadButton_clicked();
    void connectDB(QString filename);
    void queryDB();

};

#endif // DATALOAD_H


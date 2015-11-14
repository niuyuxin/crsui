
#include "configdialog.h"
#include <QSettings>

ConfigDialog::ConfigDialog(QWidget *p)
    :QDialog(p)
{
    setupUi(this);
}


ConfigDialog::ConfigDialog(QWidget *p, QStringList itemStringList)
    :QDialog(p)
{
    QSettings settings("DoLong Inc", "Command");
    setupUi(this);

    QIntValidator *intValidator = new QIntValidator(1, 9999, this);

    pressureLineEdit->setValidator(intValidator);
    timeLineEdit->setValidator(intValidator);

    QStringList columnName;
    columnName << tr("SPEED") << tr("COUNT") << tr("FREQ");
    tableWidget->setColumnCount(columnName.size());
    tableWidget->setRowCount(itemStringList.size());
    for (int i = 0; i < columnName.size(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(columnName.at(i));
        tableWidget->setHorizontalHeaderItem(i, item);
    }
    for (int i = 0; i < itemStringList.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(itemStringList.at(i));
        tableWidget->setVerticalHeaderItem(i, item);
    }
    for (int i = 0; i < columnName.size(); ++i) {
        for (int j = 0; j < itemStringList.size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            QString firstName = tableWidget->verticalHeaderItem(j)->text();
            QString lastName = tableWidget->horizontalHeaderItem(i)->text();
            item->setText(settings.value(firstName+lastName,
                                         defaultValue(firstName, lastName)).toString());
            tableWidget->setItem(j, i, item);
        }
    }
    pressureLineEdit->setText(settings.value("CLEANPRESS", 100).toString());
    timeLineEdit->setText(settings.value("CLEANTIME", 100).toString());
//    connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
//            this, SLOT(tableWidget_itemChanged(QTableWidgetItem*)));
}
int ConfigDialog::defaultValue(QString first, QString last)
{
    struct keyvalue_t {
        QString key;
        int value[4];
    }keyvalue[] = {
        {"Leak test", 0, 300, 1000, 17},
        {"VL", 0, 600, 1000, 17},
        {"EM", 0, 400, 1000, 20},
        {"LL", 0, 300, 1000, 30},
        {"VE", 0, 300, 1500, 40}
    };
    int column;
    column = (last == "SPEED") ? 1 :
            (last == "COUNT") ? 2 :
            (last == "FREQ") ? 3 : 0;
    for (uint i = 0; i < sizeof(keyvalue)/sizeof(keyvalue_t); ++i) {
        if (first.contains(keyvalue[i].key)) {
            return keyvalue[i].value[column];
        }
    }
    return keyvalue[1].value[column];
}

void ConfigDialog::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    QSettings settings("DoLong Inc", "Command");
    QString keyValue = tableWidget->verticalHeaderItem(item->row())->text();
    keyValue += tableWidget->horizontalHeaderItem(item->column())->text();
    settings.setValue(keyValue, item->text());
}
void ConfigDialog::accept()
{
    QSettings settings("DoLong Inc", "Command");
    settings.setValue("CLEANTIME", timeLineEdit->text());
    settings.setValue("CLEANPRESS", pressureLineEdit->text());
    close();
}

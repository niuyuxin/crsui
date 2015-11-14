
#include "parameterwin.h"
#include "ui_parameterwin.h"
#include "cell.h"


ParameterWin::ParameterWin(QWidget *p)
    :QDialog(p)
{

    setupUi(this);
    creatTableItem(userCommand, Command::commandSize());

    connect(&timer, SIGNAL(timeout()),
            this, SLOT(flushData()));
    timer.start(FLUSHTIME);

}
void ParameterWin::creatTableItem(command_attr_t *commands, quint16 size)
{
    QVector <command_attr_t *> cVector;
    QStringList table;
    qDebug() << "command size = " << size;
    for (int i = 0; i < size; ++i) {
        if (!commands[i].tableName.isEmpty()) {
            if (!table.contains(commands[i].tableName)) {
                table.append(commands[i].tableName);
            }
        }
    }
    for (int i = 0; i < table.size(); ++i) {
        cVector.clear();
        for (int j = 0; j < size; ++j) {
            if (commands[j].tableName == table.at(i)) {
                cVector.append(&commands[j]);
            }
        }
        QTableWidget *tableWidget = new QTableWidget;
        tableWidgetVector.append(tableWidget);
        QStringList row, column;
        for (int i = 0; i < cVector.size(); ++i) {
            if (!column.contains(cVector[i]->paraName)) {
                column << cVector[i]->paraName;
            }
            if (!row.contains(cVector[i]->columnName)) {
                row << cVector[i]->columnName;
            }
        }
        tableWidget->setColumnCount(row.size());
        tableWidget->setRowCount(column.size());
        tableWidget->setHorizontalHeaderLabels(row);
        tableWidget->setVerticalHeaderLabels(column);
        for (int i = 0, k = 0; i < row.size(); ++i) {
            for (int j = 0; j < column.size(); ++j) {
                Cell *item = new Cell(cVector[k]);
                item->setTextAlignment(Qt::AlignCenter);
                item->setToolTip(cVector[k]->comment);
                item->setText(cVector[k++]->paraValue.toString());
                tableWidget->setItem(j, i, item);
            }
        }
        connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)),
                    this, SLOT(somethingChanged(QTableWidgetItem *)));
        if ((cVector[0]->paraAttr&WRITE) != WRITE) {
            tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
        tabWidget->addTab(tableWidget, cVector[0]->tableName);
    }

}
void ParameterWin::flushData()
{
    int whichtab = tabWidget->currentIndex();
    QVector <command_attr_t *> cVector;
    QString tabName = tabWidget->tabText(whichtab);

    for (int j = 0; j < Command::commandSize(); ++j) {
        if (userCommand[j].tableName == tabName) {
            cVector.append(&userCommand[j]);
        }
    }

    for (int i = 0, k = 0; i < tableWidgetVector[whichtab]->columnCount(); ++i) {
        for (int j = 0; j < tableWidgetVector[whichtab]->rowCount(); ++j) {
            tableWidgetVector[whichtab]->item(j, i)->setText(cVector[k++]->paraValue.toString());
        }
    }
}

void ParameterWin::itemCheck()
{


}

void ParameterWin::somethingChanged(QTableWidgetItem *twi)
{
    QSettings settings("DoLong Inc", "Command");

    QVector <command_attr_t *> cVector;
    QString tabName = tabWidget->tabText(tabWidget->currentIndex());

    for (int j = 0; j < Command::commandSize(); ++j) {
        if (userCommand[j].tableName == tabName) {
            cVector.append(&userCommand[j]);
        }
    }

    QTableWidget * tableWidget = twi->tableWidget();
    int whichitem = tableWidget->rowCount() * twi->column() + twi->row();
    QString string = cVector.at(whichitem)->commandName;

    cVector.at(whichitem)->paraValue = twi->text();
    if (cVector.at(whichitem)->paraAttr & SAVE) { //存储数据
        settings.setValue(string, twi->text());
    }
    float value = twi->text().toFloat();
    int setValue = value * cVector.at(whichitem)->paraCoe;
    //serialPortThread->addTransaction(requestToByteArray(string, QString::number(setValue)));
    emit sendData(Command::requestToByteArray(string, QString::number(setValue)));
    qDebug() << string << "= " << QString::number(setValue);
}

void ParameterWin::writeSettings()
{

}
void ParameterWin::closeEvent(QCloseEvent *)
{
}
void ParameterWin::showEvent(QShowEvent *)/*showEvent */
{

}

void ParameterWin::response(const QString &str)
{
    if (str.left(QString("This is serialPort test text!").size())
            == "This is serialPort test text!")
        qDebug() << str;
}
void ParameterWin::timeout(const QString &str)
{
    qDebug() << str;
}

/**
 * =================================================================================
 * file name: detailForm.cpp
 * author: niuxin
 * company: hand
 * data: 2015/09/23
 * brief:
 *      FIXME: 1) receiveDataFromDataBase 接收上一个画面的数据为指针变量， 可能会有问题
 *             3) dataBaseOk不完善
 * =================================================================================
 */
#include "detailform.h"
#include "command.h"

DetailForm::DetailForm(QWidget *p)
    :QWidget(p)
{
    setupUi(this);
    dataBaseOk = false;
    connectInfoLabel->setText("You can click \"LOAD\" button to select your Injector type!");
    hide();
}
void DetailForm::systemTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString time = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    time.replace(10, 1, '\n');
    timeLabel->setText(time);
}
void DetailForm::on_continueButton_clicked()
{
    qDebug() << "DetailForm::continueButtonClicked()";
    hide();
    nextWidget->show();
}
void DetailForm::on_backButton_clicked()
{
    qDebug() << "DetailForm:: back buttonClicked()";
    hide();
    previousWidget->show();

}
void DetailForm::on_loadButton_clicked()
{
    this->hide();
    emit showDateBase();
}
void DetailForm::receiveDataFromDataBase(QList <QTableWidgetItem *> * twil)
{
    connectInfoLabel->setText("The data that you selected is here,\n"
                              "then you can click the \"CONTINUE\" button to do your test!");
    commentLineEdit->setReadOnly(false);
    manufactureLineEdit->setReadOnly(false);
    controlLineEdit->setReadOnly(false);
    partnumberLineEdit->setText(twil->at(0)->data(Qt::DisplayRole).toString());
    commentLineEdit->setText(twil->at(1)->data(Qt::DisplayRole).toString());
    manufactureLineEdit->setText(twil->at(2)->data(Qt::DisplayRole).toString());
    controlLineEdit->setText(twil->at(4)->data(Qt::DisplayRole).toString());
    commentLineEdit->setReadOnly(true);
    manufactureLineEdit->setReadOnly(true);
    controlLineEdit->setReadOnly(true);

    // 添加零件号 控制属性
    itemDetail.clear();
    QStringList stringList;
    stringList << partnumberLineEdit->text();
    itemDetail.append(stringList);
    stringList = getControlFeature("CriBoschProfiles", twil->at(4)->data(Qt::DisplayRole).toString());
    QString str = stringList.at(11);
    stringList.removeAt(8);
    stringList.removeAt(8);
    stringList.insert(8, str);
    for (int i = 1; i < 8; ++i) {  // 数据取舍
        str = stringList.at(i);
        stringList.removeAt(i);
        stringList.insert(i, QString::number(str.toDouble()/1000, 'f', 0));
    }
    str = stringList.at(9);
    QString temp = stringList.at(10);
    stringList.removeAt(9);
    stringList.insert(9, QVariant(str.toUInt() -  temp.toUInt()).toString());
    QStringList stringListCommand;
    stringListCommand << "InjectorControlType"
                      << "InjectorControlVolatge"
                      << "InjectorEnhanceVoltage"
                      << "InjectorEnhanceCurrent"
                      << "InjectorShakeCurrent"
                      << "InjectorShakeCurrentError"
                      << "InjectorKeepCurrent"
                      << "InjectorKeepCurrentError"
                      << "InjectorEnhanceTime"
                      << "InjectorShakeTime"
                      << "InjectorMinKeepTime"
                      << "InjectorCloseType";
    for (int i = 0; i < stringListCommand.size(); ++i) {
        Command::updateCommandValue(stringListCommand.at(i), stringList.at(i));
    }

    // 创建表格
    QVector<QStringList> stringListVector =
            getOperatingMode("CriPruefschritte", twil->at(0)->data(Qt::DisplayRole).toString());
//    for (int i = 0; i < stringListVector.size(); i++) {
//        qDebug() << QString("stringListVector.at(%1).at(3)").arg(i) << stringListVector[i].at(2);
//    }

    stringList.clear();
    stringList << tr("TEST STEPS NAME")
               //<< tr("TEST STEPS TYPE")
               << tr("CONTROL TIME")
               << tr("PRESURE")
               << tr("FUEL INJECTION")
               << tr("ERROR")
               << tr("FUEL RETURN")
               << tr("ERROR")
               << tr("TEST TIME");
    creatTable(stringListVector, stringList);
    collectItemData();
    continueButton->setEnabled(dataBaseOk = true);
}

void DetailForm::hideEvent(QHideEvent *)
{
    emit showLoadButton(false);
}

void DetailForm::showEvent(QShowEvent * event)
{
    qDebug() << "DetailForm::showEvent";
    continueButton->setEnabled(dataBaseOk);
    backButton->setEnabled(previousWidget);
    QWidget::showEvent(event);

}
void DetailForm::resizeEvent(QResizeEvent *)
{
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < tableWidget->columnCount() - 1; i++)
        tableWidget->setColumnWidth(i, rect.width()/tableWidget->columnCount());
}
void  DetailForm::widgetPrevNext(QWidget *prev, QWidget *next)
{
    nextWidget = next;
    previousWidget = prev;
}
QStringList DetailForm::getControlFeature(const QString &fileName, const QString &item)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("DataBase"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return QStringList();
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    quint32 magic;
    in >> magic;
    if (magic != 0x12345678) {
        QMessageBox::warning(this, tr("DataBase"),
                             tr("The file is not a DataBase file."));
        return QStringList();
    }
    quint32 column;
    in >> column;
    QString s;
    QStringList sl;
    for (quint32 i = 0; i < column; i++) {
        in >> s;
        sl << s;
    }
    quint32 row;
    in >> row;
    sl.clear();
    for (quint32 i = 0; i < row; i++) {
        in >> s;
        if (s == item) {
            sl << s;
            for (quint32 j= 1; j < column; j++) {
                in >> s;
                sl << s;
            }
            break;
        } else {
            for (quint32 j= 1; j < column; j++) {
                in >> s;
            }
        }
    }
    return sl;
}
QVector <QStringList> DetailForm::getOperatingMode(const QString &fileName, const QString &item)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("DataBase"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return QVector<QStringList>();
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);

    quint32 magic;
    in >> magic;
    if (magic != 0x12345678) {
        QMessageBox::warning(this, tr("DataBase"),
                             tr("The file is not a DataBase file."));
        return QVector<QStringList>();
    }
    quint32 column;
    in >> column;
    QString string;
    QStringList stringList;
    for (quint32 i = 0; i < column; i++) {
        in >> string;
    }
    quint32 row;
    in >> row;
    QVector<QStringList> stringListVector;
    for (quint32 i = 0; i < row; i++) {
        in >> string;
        in >> string;
        if (item == string) {
            stringList.clear();
            stringList << string;
            for (quint32 j= 2; j < column; j++) {
                in >> string;
                stringList << string;
            }
            stringListVector.append(stringList);
        } else {
            for (quint32 j= 2; j < column; j++) {
                in >> string;
            }
        }
    }
    return stringListVector;
}
void DetailForm::creatTable(QVector<QStringList> & stringListVector, QStringList &stringList)
{
    tableWidget->clear();
    int row = stringListVector.size();
    int column = stringList.size();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);
    tableWidget->setRowCount(row);
    tableWidget->setColumnCount(column);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers (QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
//    tableWidget->verticalHeader()->setStretchLastSection(true);
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < tableWidget->columnCount() - 1; i++)
        tableWidget->setColumnWidth(i, rect.width()/tableWidget->columnCount());

    tableWidget->setHorizontalHeaderLabels(stringList);

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            setText(i, j, stringListVector.at(i).at(j+2));
        }
    }
//    for (int i = 0; i < column; ++i) {
//        QTableWidgetItem *item = new QTableWidgetItem;
//        item->setText(sl[i]);
//        tableWidget->setHorizontalHeaderItem(i, item);
//    }
    //tableWidget->setCurrentCell(0, 0);
}
void DetailForm::setText(int row, int column, const QString &text)
{
    QTableWidgetItem *c = tableWidget->item(row, column);
    QString string = text;
    if (string.contains('.')) string = string.number(string.toDouble(), 'g', 4);
    if (string.compare("nan", Qt::CaseInsensitive) == 0) string = "---";
    if (!c) {
        c = new QTableWidgetItem;
        c->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, column, c);
    }
    c->setData(Qt::EditRole, string);
}
void DetailForm::collectItemData()
{
    QStringList stringList;

    for (int i = 0; i < tableWidget->rowCount(); i++) { // 获取需要测试的数据
        QString string = tableWidget->item(i, 3)->data(Qt::DisplayRole).toString();
        if (string.compare("---", Qt::CaseInsensitive) != 0) {
            stringList << string;
            stringList << tableWidget->item(i, 4)->data(Qt::DisplayRole).toString();
            for (int j = 2; j >= 0; j--) {
                stringList.insert(0, tableWidget->item(i, j)->data(Qt::DisplayRole).toString());
            }
            itemDetail.append(stringList);
            stringList.clear();
        }
        string = tableWidget->item(i, 5)->data(Qt::DisplayRole).toString();
        if (string.compare("---", Qt::CaseInsensitive) != 0) {
            stringList << string;
            stringList << tableWidget->item(i, 6)->data(Qt::DisplayRole).toString();
            for (int j = 2; j >= 0; j--) {
                if (j == 0) {
                    stringList.insert(0, tableWidget->item(i, j)->data(Qt::DisplayRole).toString().append("-R"));
                } else {
                    stringList.insert(0, tableWidget->item(i, j)->data(Qt::DisplayRole).toString());
                }
            }
            itemDetail.append(stringList);
            stringList.clear();
        }
    }
    emit sendDetailItemData(itemDetail);
}

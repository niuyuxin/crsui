
#include "datebaseform.h"

DateBaseForm::DateBaseForm(QWidget *p)
    :QWidget(p)
{
    setupUi(this);
    dataBaseFileName  = QString("CriKomponenten");
//    dataBaseFileName = QString("CriPruefschritte");
    connectInfoLabel->setText("You can input some information about your Injector "
                              "to the \"model-partnumber\" !");
    this->hide();
}
void DateBaseForm::systemTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString time = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    time.replace(10, 1, '\n');
    timeLabel->setText(time);
}
void DateBaseForm::on_continueButton_clicked()
{
    hide();
    nextWidget->show();
}
void DateBaseForm::on_backButton_clicked()
{
    hide();
    previousWidget->show();
}

void DateBaseForm::hideEvent(QHideEvent *event)
{
    emit showDateBaseButton(false);
    qDebug() << "DateBaseForm::hideEvent(QShowEvent *)";
    tableWidget->clear();
    QWidget::hideEvent(event);
}
void DateBaseForm::showEvent(QShowEvent *)
{
    continueButton->setEnabled(nextWidget);
    backButton->setEnabled(previousWidget);
    search(partnumberLineEdit->text());
}

void DateBaseForm::resizeEvent(QResizeEvent *)
{
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < tableWidget->columnCount() - 1; i++)
        tableWidget->setColumnWidth(i, rect.width()/5);
}

void  DateBaseForm::widgetPrevNext(QWidget *prev, QWidget *next)
{
    nextWidget = next;
    previousWidget = prev;
}
bool DateBaseForm::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("DataBase"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);

    quint32 magic;
    in >> magic;
    if (magic != 0x12345678) {
        QMessageBox::warning(this, tr("DataBase"),
                             tr("The file is not a DataBase file."));
        return false;
    }


    quint32 column;
    in >> column;
    QString s;
    QStringList sl;
    for (quint32 i = 0; i < column; i++) {
        in >> s;
        if (i > 0 && i < 6)
            sl << s;
    }
    quint32 row;
    in >> row;
    creatTable(row, sl.size(), sl);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (quint32 i = 0; i < row; i++) {
        for (quint32 j= 0; j < column; j++) {
            in >> s;
            if (j > 0 && j < 6) {
                setText(i, j-1, s);
            }
        }
    }
    QApplication::restoreOverrideCursor();
//    tableWidget->setCurrentCell(0, 0);
    file.close();
    return true;
}

void DateBaseForm::creatTable(int row, int column, QStringList &sl)
{
    tableWidget->clear();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);
    tableWidget->setRowCount(row);
    tableWidget->setColumnCount(column);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers (QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < 4; i++)
        tableWidget->setColumnWidth(i, rect.width()/5);

    for (int i = 0; i < column; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(sl[i]);
        tableWidget->setHorizontalHeaderItem(i, item);
    }

    //tableWidget->setCurrentCell(0, 0);
}
void DateBaseForm::setText(int row, int column, const QString &text)
{
    QTableWidgetItem *c = tableWidget->item(row, column);
    if (!c) {
        c = new QTableWidgetItem;
        c->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, column, c);
    }
    c->setData(Qt::EditRole, text);
}
void DateBaseForm::on_tableWidget_itemSelectionChanged()
{
    /* 函数结束时， 信号传递的参数还存在吗？ FIXME */
    QList<QTableWidgetItem *>  twil;
    twil = tableWidget->selectedItems();
    if (twil.empty()) {
        return;
    }
    QString partnumberText = twil.at(0)->data(Qt::DisplayRole).toString();
    QString vendorText = twil.at(2)->data(Qt::DisplayRole).toString();
    QString modelText = twil.at(4)->data(Qt::DisplayRole).toString();
    QString commentText = twil.at(1)->data(Qt::DisplayRole).toString();
    vendorLabel->setText(vendorText);
    modelLabel->setText(modelText);
    commentLabel->setText(commentText);

    emit dateBaseSelect(&twil);
}
void DateBaseForm::on_tableWidget_cellDoubleClicked(int /*row */, int /*column*/)
{
    continueButton->click();
}

void DateBaseForm::search(const QString &str)
{
    if (!readFile(dataBaseFileName)) {
        return;
    }
    tableWidget->sortItems(0);  // sort table widget
    quint16 row = tableWidget->rowCount();

    QList <QTableWidgetItem *> twil;
    twil = tableWidget->findItems(str, Qt::MatchContains);

    if (twil.empty()) {
        for (quint16 i = 0; i < row; i++)  {
            tableWidget->removeRow(0);
        }
        return;
    }
    QList <quint16> list;
    for (int i = 0; i < twil.size(); i++) {
        list.append(twil.at(i)->row());
    }
//    tableWidget->setCurrentCell(twil.at(0)->row(), twil.at(0)->column());
    for (quint16 i = 0, k = 0; i < row; i++) {
        if (list.size()) {
            for (quint16 j = 0; j < list.size(); j++) {
                if (i == list.at(j)) {
                    list.removeAt(j);
                    k++;
                    break;
                } else {
                    tableWidget->removeRow(k);
                    break;
                }
            }
        } else {
           tableWidget->removeRow(k);
        }
    }

}
void DateBaseForm::on_searchButton_clicked()
{
    //search(partnumberLineEdit->text());
}
void DateBaseForm::on_partnumberLineEdit_textChanged(const QString &str)
{
    vendorLabel->setText("");
    modelLabel->setText("");
    commentLabel->setText("");
    search(str);
}


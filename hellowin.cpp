#include <QtWidgets>
#include "hellowin.h"



HelloWin::HelloWin(QWidget *p)
    :QWidget(p)
{
    setupUi(this);

    beginTreeWidget->expandItem(beginTreeWidget->itemAt(0, 0));
    beginTreeWidget->itemBelow(beginTreeWidget->itemAt(0, 0))->setSelected(true);
    beginTreeWidget->setCurrentItem(beginTreeWidget->itemBelow(beginTreeWidget->itemAt(0, 0)));
    serialPortSet = new SerialPortSet(this);
    versionLabel->setText("Ver. 15.10.0");
    langDialog = new LangDialog(this);
    dataLoad = new DataLoad(this);
    connect(serialPortSet, SIGNAL(serialPortChanged(QString)),
            this, SIGNAL(serialPortChanged(QString)));

    this->hide();
}

void HelloWin::systemTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString time = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    time.replace(10, 1, '\n');
    timeLabel->setText(time);
}

void HelloWin::on_beginTreeWidget_itemDoubleClicked(QTreeWidgetItem *twi, int n)
{
    QString text = twi->text(n);
    if (text == "CRI/CRIN") {
        this->hide();
        nextWidget->show();
    }
    if (text == "CLEAR ALL CONFIG") {
        int ret = QMessageBox::warning(this, tr("Clear Applications Config"),
                                       tr("Click \"Yes\" Button to clear config!\n"
                                          "Do you want to clear?"),
                                       QMessageBox::Cancel,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::Yes) {
            QSettings settings("DoLong Inc.", "CRS control system");
            settings.clear();
            QSettings setting("DoLong Inc", "Command");
            setting.clear();
        }
    }
    if (text == "LANGUAGE") {
        langDialog->exec();
    }
    if (text == "LOAD") {
        dataLoad->show();
    }
    if (text == "SERIAL PORT") {
        serialPortSet->exec();
    }
}
void HelloWin::on_beginTreeWidget_itemClicked(QTreeWidgetItem* /* twi */ , int /* n */ )
{
    //emit treeWidgetClicked(twi, n);
    //qDebug() << twi->text(n);
}
void HelloWin::on_continueButton_clicked()
{
    if (!this->isHidden()) {
        if (!beginTreeWidget->currentItem()) {
            return;
        }
        on_beginTreeWidget_itemDoubleClicked(beginTreeWidget->currentItem(),
                                             beginTreeWidget->currentColumn());
    }
}
void HelloWin::on_backButton_clicked()
{
//    hide();
//    previousWidget->show();
}
void HelloWin::showEvent(QShowEvent * event)
{
    continueButton->setEnabled(nextWidget);
    backButton->setEnabled(previousWidget);

    QWidget::showEvent(event);
}
void HelloWin::hideEvent(QHideEvent *)
{

}

void HelloWin::widgetPrevNext(QWidget *prev, QWidget *next)
{
    nextWidget = next;
    previousWidget = prev;
}
void HelloWin::on_connectButton_clicked()
{
    qDebug() << "connectButton clicked()";
}
void HelloWin::response(const QString &str)
{
    if (str.left(QString("This is serialPort test text!").size())
            == "This is serialPort test text!")
        connectInfoLabel->setText(str);
    else connectInfoLabel->setText("Hello!\nDevice is offline!\n Please check... ");
}
void HelloWin::timeout(const QString &str)
{
    connectInfoLabel->setText(str);
}
bool HelloWin::event(QEvent *event)
{
    return QWidget::event(event);
}



#include "dataload.h"
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlIndex>
#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
//#include <QAxObject>

DataLoad::DataLoad(QWidget *p)
    :QDialog(p)
{
    setupUi(this);
    moreGroupBox->hide();
    dataloadGridLayout->setSizeConstraint(QLayout::SetFixedSize);
}

void DataLoad::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("open file"),
                                                    ".",
                                                    tr("MS FILE (*.mdb)"));
    fileLineEdit->setText(fileName);
    
}
void DataLoad::on_loadButton_clicked()
{
    qDebug() << QFileInfo(fileLineEdit->text()).fileName();
    connectDB(fileLineEdit->text());
    queryDB();
    writeFile(profileComboBox->currentText(), profileComboBox->currentText());
    progressBar->setValue(30);
    writeFile(componentComboBox->currentText(), componentComboBox->currentText());
    progressBar->setValue(60);
    writeFile(stepComboBox->currentText(), stepComboBox->currentText());
    progressBar->setValue(100);
}
void DataLoad::connectDB(QString filename)
{
    QSqlDatabase dbconn = QSqlDatabase::database("bosch", false);

    if(dbconn.isValid()) {
        qDebug() << "dbconn.isValid";
        return;
    }
    dbconn = QSqlDatabase::addDatabase("QODBC", "bosch");
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=%1;").arg(filename);
    dbconn.setDatabaseName(dsn);
}
void DataLoad::queryDB()
{
    QSqlDatabase dbconn = QSqlDatabase::database("bosch", false);
    dbconn.open();
    QStringList sl = dbconn.tables();
    for (int i = 0; i < sl.size(); i++) {
        qDebug() << sl.at(i);
    }
    profileComboBox->addItems(sl);
    stepComboBox->addItems(sl);
    componentComboBox->addItems(sl);
    for (int i = 0; i < profileComboBox->count(); i++) {
        if (profileComboBox->itemText(i) == "CriBoschProfiles")
            profileComboBox->setCurrentIndex(i);
        if (stepComboBox->itemText(i) == "CriPruefschritte")
            stepComboBox->setCurrentIndex(i);
        if (componentComboBox->itemText(i) == "CriKomponenten")
            componentComboBox->setCurrentIndex(i);
    }
}
bool DataLoad::writeFile(const QString &fileName, const QString &tablename)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint32(0x12345678);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QSqlDatabase dbconn = QSqlDatabase::database("bosch");

    QSqlQuery *query = new QSqlQuery(dbconn);
    query->exec(QString("SELECT * FROM %1").arg(tablename));
    QSqlRecord sqlRecord;
    sqlRecord = query->record();
    out << quint32(sqlRecord.count());
    for (int i = 0; i < sqlRecord.count(); i++) {
        out << sqlRecord.fieldName(i);
    }
    quint32 num = 0;
    while (query->next()) num++;
    out << quint32(num);
    query->first();
    num = 0;
    do {
        for (int i = 0; i < sqlRecord.count(); i++) {
            out  << query->value(i).toString();
        }
    } while (query->next());

    QApplication::restoreOverrideCursor();
    return true;
}


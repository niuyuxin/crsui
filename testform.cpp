#include "testform.h"
#include "command.h"

TestForm::TestForm(QWidget *parent)
    :QWidget(parent)
{
    QSettings settings("DoLong Inc", "Command");
    setupUi(this);
    currentTestItem << tr("CLEAN")                                   // 0: 工况
                    << settings.value("CLEANSPEED", "123").toString() // 1: 转速
                    << settings.value("CLEANCOUNT", "1234").toString() // 2:
                    << settings.value("CLEANTIME", "300").toString() // 3 时间
                    << settings.value("CLEANPRESS", "50").toString() // 4 轨压
                    << settings.value("CLEANFREQ", "17").toString();   // 5 喷油器频率
    // 设置runButton 可以checkable
    runButton->setCheckable(true);
    oilpumpButton->setCheckable(true);

    // 模拟特性曲线 TODO
    plotter = new Plotter(this);
    int numPoints = 1500;
    for (int x = 0; x < numPoints; x += 10) {
        plotPoints.append(QPoint(x, 100));
    }
    plotter->setCurveData(0, plotPoints);
    plotSettings.minX = 0.0;
    plotSettings.maxX = 1500.0;
    plotSettings.minY = 0.0;
    plotSettings.maxY = 200.0;
    plotter->setPlotSettings(plotSettings);

    plotLayout->addWidget(plotter);

    QIntValidator validator(3, 99);
    tempSetLineEdit->setValidator(&validator);


    thread = new Thread(this);
    connect(this, SIGNAL(testItems(QVector<QStringList>&)),
            thread, SLOT(getTestItems(QVector<QStringList>&)));
    connect(thread, SIGNAL(finishedTest(QString)),
            this, SLOT(error(QString)));
    connect(oilpumpButton, SIGNAL(clicked(bool)),
            thread, SLOT(oilpumpButtonClicked(bool)));
    connect(thread, SIGNAL(updateValue(QString,QString)),
            this, SLOT(updateValue(QString,QString)));
    connect(thread, SIGNAL(currentWorkingCondition(QStringList)),
            this, SLOT(currentWorkingCondition(QStringList)));
    connect(thread, SIGNAL(updatePlot(int,float)),
            this, SLOT(updatePlot(int,float)));
    testStepsLabel->setText(tr("The equipment is not running!"));
    hide();
}
TestForm::~TestForm()
{
    delete thread;
    qDebug() << "TestForm::~TestForm()";
}
void TestForm::systemTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString time = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    time.replace(10, 1, '\n');
    timeLabel->setText(time);
}
void TestForm::updateValue(QString item, QString value)
{
    if (item == "TemperatureValue") {
        temperatureValueLabel->setText(value);

    }
    if (item == "SpeedValue") {
        speedValueLabel->setText(value);
    }
    if (item == "InjFlowPulseNum") {
        int flowNum = value.toInt();
        int coe = Command::getCommand("InjFlowSensorCoe")->paraValue.toInt();
        QString freq = currentTestItem.at(5);
        int injFreq = freq.toInt();
        if (injFreq == 0) injFreq = 17;
        float flowValue = float(flowNum * 50000) / (3 * coe * injFreq);
        if (flowValue > 200) flowValue = 200;
        Command::updateCommandValue("InjFlowValue", flowValue);
        flowValueLabel->setText(QString::number(flowValue, 'f', 1));
    }
    if (item == "RailPressure") {
        railPressureValueLabel->setText(value);
    }
    if (item == "InjectorCountValue") {
        countLabel->setText(value);
        countProgressBar->setValue(value.toInt());
        if (currentTestItem.at(0) != "CLEAN" ) { // FIXME: weird number of “6”
            QString index = currentTestItem.at(currentTestItem.size()-1);
            tableWidget->item(index.toInt(), 5)->setText(flowValueLabel->text());
        }
    }
}
void TestForm::on_tempSetLineEdit_returnPressed()
{
    qDebug() << "lineEdit()" << tempSetLineEdit->text();
    emit sendData(Command::requestToByteArray("BenchTempSet", tempSetLineEdit->text()));
}
void TestForm::on_tempSlider_valueChanged(int value)
{
    qDebug() << "tempSlider" << value;
    Command::updateCommandValue("TControl", value);
    Command::updateSystemControlInfo();
    emit sendData(Command::requestToByteArray("SystemControlInfo"));
}

void TestForm::on_continueButton_clicked()
{
    qDebug() << "xxxxxTestForm";
    hide();
    nextWidget->show();
    qDebug() << "TestForm::continueButtonClicked()";
}
void TestForm::on_backButton_clicked()
{
    qDebug() << "TestForm:: back buttonClicked()";
    hide();
    previousWidget->show();

}
void TestForm::on_runButton_toggled(bool checked)
{
    QStringList stringList;
    QSettings settings("DoLong Inc", "Command");
    QVector <QStringList> testItemVector;
    if (checked) { // 按键按下
        if (cleanButton->isChecked()) {
            stringList << tr("CLEAN");                                   // 0: 工况
            stringList << settings.value("CLEANSPEED", "500").toString(); // 1: 转速
            stringList << settings.value("CLEANCOUNT", "100").toString(); // 2:
            stringList << settings.value("CLEANTIME", "100").toString(); // 3 时间
            stringList << settings.value("CLEANPRESS", "100").toString(); // 4 轨压
            stringList << settings.value("CLEANFREQ", "17").toString();    // 5 喷油器频率
            stringList << QString::number(9);                            // 5
            testItemVector.append(stringList);
        }
        QString flow("---");
        for (int i = 0; i < tableWidget->rowCount(); ++i) {
            tableWidget->item(i, 5)->setText(flow);
            if (tableWidget->item(i, 0)->isSelected()) {
                stringList.clear();
                stringList << tableWidget->item(i, 0)->text();              // 0: 工况
                QString key = tableWidget->item(i, 0)->text();
                stringList << settings.value(key+"SPEED", "500").toString(); // 1: 转速
                stringList << settings.value(key+"COUNT", "1000").toString(); // 2:
                stringList << tableWidget->item(i, 1)->text();                // 3 时间
                stringList << tableWidget->item(i, 2)->text();                // 4 轨压
                stringList << settings.value(key+"FREQ", "17").toString();    // 5 喷油器频率
                stringList << QString::number(i);                             // 6 工况号
                testItemVector.append(stringList);
            }
        }
        backButton->setEnabled(false);
        runButton->setIcon(QIcon(":/images/EPS_Stop.png"));
        runLabel->setText(tr("STOP"));
        on_tableWidget_cellClicked(0, 0);
        emit testItems(testItemVector);
        //thread->runTest();
        if (Command::getCommand("MotoDirection")->paraValue.toBool()) {
            Command::updateCommandValue("MotoFwd", "0");
            Command::updateCommandValue("MotoRev", "1");
        } else {
            Command::updateCommandValue("MotoFwd", "1");
            Command::updateCommandValue("MotoRev", "0");
        }
        Command::updateCommandValue("InjectorControl", "1");
        Command::updateCommandValue("CRPControl", "1");
        Command::updateCommandValue("DRVSwitch", "1");

        Command::updateSystemControlInfo();
        tableWidget->setEnabled(false);
    } else {
        //Command::updateCommandValue("InjectorControl", "0");
        Command::updateCommandValue("CRPControl", "0");
        Command::updateCommandValue("DRVSwitch", "0");
        Command::updateCommandValue("MotoFwd", "0");
        Command::updateCommandValue("MotoRev", "0");
        Command::updateCommandValue("FuelSupplyEMV", "0");
        Command::updateCommandValue("FuelReturnEMV", "0");
        Command::updateCommandValue("InjectorCountSwitch", "0");
        Command::updateSystemControlInfo();

        thread->stopTest();
        testStepsLabel->setText(tr("The equipment is not running!"));
        backButton->setEnabled(true);
        runButton->setIcon(QIcon(":/images/EPS_Start.png"));
        runLabel->setText(tr("START"));
        tableWidget->setEnabled(true);
    }

}
void TestForm::currentWorkingCondition(QStringList current)
{
    plotPoints.clear();
    currentTestItem = current;
    setSpeedLabel->setText(current.at(1));
    QString value = current.at(2);
    plotSettings.maxX = value.toInt();  //


    countLabel->setText(value);
    countProgressBar->setValue(0);
    countProgressBar->setRange(0, value.toUInt());
    value = current.at(4);
    qDebug() << "current" << current;
    qDebug() << "value = " << value;
    plotSettings.maxY = value.toFloat() * 2;

    testStepsLabel->setText(QString("%1 item is testing!").arg(current.at(0)));
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        if (tableWidget->item(i, 0)->text() == current.at(0)) {
            for (int j = 0; j < tableWidget->columnCount(); ++j) {
                tableWidget->item(i, j)->setSelected(true);
            }
            return;
        }
    }
}
void TestForm::updatePlot(int count, float flow)
{
    float c = count;
    QPointF point(c, flow);
    plotPoints.append(point);
    plotter->setCurveData(0, plotPoints);
    plotter->setPlotSettings(plotSettings);
}

void TestForm::showEvent(QShowEvent * event)
{

    if (partNumber != itemGroupBox->title()) {
        partNumber = itemGroupBox->title();
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
            emit sendData(Command::requestToByteArray(stringListCommand.at(i)));
        }
    }
    continueButton->setEnabled(nextWidget);
    backButton->setEnabled(previousWidget);
    thread->start();
    QWidget::showEvent(event);
    // FIXME:
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < tableWidget->columnCount() - 1; i++)
        tableWidget->setColumnWidth(i, rect.width()/tableWidget->columnCount());
    qDebug() << "TestForm::showEvent";
}
void TestForm::hideEvent(QHideEvent *)
{
    thread->stop();
}

void TestForm::resizeEvent(QResizeEvent *)
{
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < tableWidget->columnCount() - 1; i++)
        tableWidget->setColumnWidth(i, rect.width()/tableWidget->columnCount());
}
void TestForm::on_tableWidget_cellClicked(int row, int column)
{

    QString selectedItem;
    QString flow("---");
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        tableWidget->item(i, 5)->setText(flow);
        if (tableWidget->item(i, 0)->isSelected()) {
            selectedItem += "  ";
            selectedItem += '\"';
            selectedItem += tableWidget->item(i, 0)->text();
            selectedItem += '\"';
        }
        for (int j = 1; j < tableWidget->columnCount(); ++j) {
            tableWidget->item(i, j)->setSelected(false);
        }
    }
    if (column == 0) {
        if (tableWidget->item(row, 0)->isSelected())
           testStepsLabel->setText(QString("\"%1\"  is selection! Current Selected item(s) is %2")
                                   .arg(tableWidget->item(row, column)->text())
                                   .arg(selectedItem));
        else {
            if (!selectedItem.isEmpty())
                testStepsLabel->setText(QString("Cancel selection \"%1\" item! Current Selected item(s) is %2")
                                    .arg(tableWidget->item(row, column)->text())
                                    .arg(selectedItem));
            else
                testStepsLabel->setText(tr("please select your item(s)!"));
        }
    }

}
void TestForm::on_tableWidget_itemChanged(QTableWidgetItem * /* item */)
{
//    if ((item->column() != 0) && (item->column() != (item->tableWidget()->columnCount() -1))) {
//        QString text = item->text();
//        QIntValidator *v = new QIntValidator(0, 999);
//        int q = 0;
//        if (v->validate(text, q) != QValidator::Acceptable) {
//            text = "";
//            item->setText(text);
//        }
//    }
}

void  TestForm::widgetPrevNext(QWidget *prev, QWidget *next)
{
    nextWidget = next;
    previousWidget = prev;
}
void TestForm::receiveDataFromItemDetail(QVector<QStringList> &stringListVector)
{
//    for (int i = 0; i < stringListVector.size(); i++) {
//        qDebug() << stringListVector.at(i);
//    }
    QStringList stringList;
    stringList << tr("STEPS") << tr("TIME") << tr("PRESURE") << tr("FUEL") << tr("TOLERANCE") << tr("VALUE");
    creatTable(stringListVector, stringList);
}
void TestForm::creatTable(QVector<QStringList> & stringListVector, QStringList &stringList)
{
    tableWidget->clearContents();
    itemGroupBox->setTitle(stringListVector.at(0).at(0));// 0行： 喷油器号
    stringListVector.remove(0);
    int row = stringListVector.size();
    int column = stringList.size();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);
    tableWidget->setRowCount(row);
    tableWidget->setColumnCount(column);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
//    tableWidget->setEditTriggers (QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
//    tableWidget->verticalHeader()->setStretchLastSection(true);
    QRect rect = tableWidget->geometry();
    for (int i = 0; i < tableWidget->columnCount() - 1; i++)
        tableWidget->setColumnWidth(i, rect.width()/tableWidget->columnCount());

    tableWidget->setHorizontalHeaderLabels(stringList);

    for (int i = 0, j = 0; i < row; ++i) {
        for (j = 0; j < column-1; ++j) {
            setText(i, j, stringListVector.at(i).at(j));
        }
        setText(i, j, "---");
    }
}
void TestForm::setText(int row, int column, const QString &text)
{
    QTableWidgetItem *c = tableWidget->item(row, column);
    QString string = text;
    if (!c) {
        c = new QTableWidgetItem;
        c->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, column, c);
    }
    c->setData(Qt::EditRole, string);
}
void TestForm::response(const QString &str)
{
//    QString address = str;
//    QString data = str;

    if (str.left(QString("This is serialPort test text!").size())
            == "This is serialPort test text!")
        connectInfoLabel->setText(str);
    else connectInfoLabel->setText("Hello!\nDevice is offline!\n Please check... ");


}
void TestForm::timeout(const QString &str)
{
    connectInfoLabel->setText(str);

}
void TestForm::error(const QString &str)
{
    if (str == "error") {
        runButton->setChecked(false);
        QMessageBox::warning(this, "Warning", tr("please select you test item(s)!"));
    } else if (str == "testEnd") {
        testStepsLabel->setText("test end!");
        runButton->setChecked(false);
    } else {
        connectInfoLabel->setText(str);
    }
}
void TestForm::on_configButton_clicked()
{
    QStringList sl;
    sl << "CLEAN";
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        sl << tableWidget->item(i, 0)->text();
    }
    configDialog = new ConfigDialog(this, sl);
    configDialog->exec();
    delete configDialog;
}

bool TestForm::event(QEvent *event)
{
    return QWidget::event(event);
}


#include "mainwindow.h"

MainWindow::MainWindow()
{
    serialPortThread = new SerialPortThread(this);
    helloWin = new HelloWin(this);
    detailForm = new DetailForm(this);
    dateBaseForm = new DateBaseForm(this);
    testForm = new TestForm(this);


    helloWin->widgetPrevNext(0, detailForm);
    detailForm->widgetPrevNext(helloWin, testForm);
    dateBaseForm->widgetPrevNext(detailForm, detailForm);
    testForm->widgetPrevNext(detailForm, 0);

    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(1, 1, 1, 1);
    widget->setLayout(layout);

    layout->addWidget(helloWin);
    layout->addWidget(detailForm);
    layout->addWidget(dateBaseForm);
    layout->addWidget(testForm);

    setCentralWidget(widget);
    helloWin->show();

    qRegisterMetaType <command_value_t> ("command_value_t");
    connect(helloWin, SIGNAL(serialPortChanged(QString)),
            serialPortThread, SLOT(serialPortChanged(QString)));
    connect(helloWin, SIGNAL(sendData(command_value_t)),
            serialPortThread, SLOT(addTransaction(command_value_t)));
    connect(testForm, SIGNAL(sendData(command_value_t)),
            serialPortThread, SLOT(addTransaction(command_value_t)));
    connect(serialPortThread, SIGNAL(error(QString)),
            testForm, SLOT(error(QString)));
    connect(serialPortThread, SIGNAL(response(command_value_t)),
            testForm->thread, SLOT(response(command_value_t)));
    connect(testForm->thread, SIGNAL(request(command_value_t)),
            serialPortThread, SLOT(addTransaction(command_value_t)));

    connect(detailForm, SIGNAL(showDateBase()),
            dateBaseForm, SLOT(show()));
    connect(dateBaseForm, SIGNAL(dateBaseSelect(QList <QTableWidgetItem *> *)),
            detailForm, SLOT(receiveDataFromDataBase(QList <QTableWidgetItem *> *)));
    connect(detailForm, SIGNAL(sendDetailItemData(QVector<QStringList>&)),
            testForm, SLOT(receiveDataFromItemDetail(QVector<QStringList>&)));

//    connect(helloWin, SIGNAL(serialPortChanged(QString)),
//            serialPortThread, SLOT(setSerialPort(QString)));
	setWindowTitle(QObject::tr("CRS TEST SYSTEM"));
    setWindowIcon(QIcon(":/images/cpyicon.png"));

    Command::readSettings();

    connect(&systemTimer, SIGNAL(timeout()), helloWin, SLOT(systemTime()));
    connect(&systemTimer, SIGNAL(timeout()), detailForm, SLOT(systemTime()));
    connect(&systemTimer, SIGNAL(timeout()), dateBaseForm, SLOT(systemTime()));
    connect(&systemTimer, SIGNAL(timeout()), testForm, SLOT(systemTime()));

    systemTimer.start(1000);

}
MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)
            && keyEvent->key() == Qt::Key_A) {
        parameterWin = new ParameterWin(this);
        connect(parameterWin, SIGNAL(sendData(command_value_t)),
                serialPortThread, SLOT(addTransaction(command_value_t)));
        parameterWin->exec();
        delete parameterWin;
    }
}

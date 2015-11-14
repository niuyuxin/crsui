#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QTime>
#include <stdio.h>
#include <stdlib.h>

#include "mainwindow.h"




void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput);

    QApplication app(argc, argv);
    MainWindow mainWin;

    QPixmap pixmap(":/images/startup.jpg");
    QSplashScreen screen(pixmap);
    screen.show();
    screen.showMessage("System is going up, please wait a minute...",
                        Qt::AlignBottom | Qt::AlignLeft, Qt::black);

    QTime time = QTime::currentTime();
    QTime now;
    do {
        now = QTime::currentTime();
       app.processEvents();
    } while (time.msecsTo(now) < 2);

    mainWin.show();

    screen.finish(&mainWin);
    return app.exec();
}

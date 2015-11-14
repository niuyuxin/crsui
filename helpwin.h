#ifndef HELPWIN_H
#define HELPWIN_H

#include <QtWidgets>
#include "ui_helpwin.h"

class HelpWin: public QDialog, public Ui::helpWin
{
    Q_OBJECT
public:
    HelpWin(QWidget *p = 0);
public slots:
    void jok();
};


#endif // HELPWIN_H

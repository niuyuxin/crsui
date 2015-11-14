#include "helpwin.h"

HelpWin::HelpWin(QWidget *p)
    : QDialog(p)
{
    setupUi(this);
    connect(asoPushButton, SIGNAL(clicked()),
                    this, SLOT(jok()));
}
void HelpWin::jok()
{
    QMessageBox::about(this, tr("About Help"),
            tr("<p> We are appologized! "
               "<p> This is a jok!"
               "<p> We are preparing the help document!"));
}

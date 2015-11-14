#ifndef LANGDIALOG_H
#define LANGDIALOG_H
#include <QtWidgets>
#include "ui_langdialog.h"

class LangDialog : public QDialog, public Ui::langDialog
{
public:
	LangDialog(QWidget *p = 0);
};




#endif

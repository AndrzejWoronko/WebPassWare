#ifndef DIALOG_H
#define DIALOG_H

#include "Global.h"
#include "Layouts.h"
#include <QDialog>
#include <QCloseEvent>

#define DEFAULT_SAVE_NAMESECTION "RuntimeSettings"

class CDialog : public QDialog
{
  Q_OBJECT
public:
  CDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  CDialog(int dx, int dy, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
protected:
  void centerWindow();

signals:

public slots:

};


#endif

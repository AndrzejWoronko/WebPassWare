#include "Dialog.h"
#include "ApplicationSettings.h"

CDialog::CDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    this->setMinimumSize(SETT.getValue(SETTINGS_GUI_DIALOG_MIN_WIDTH).toInt(), SETT.getValue(SETTINGS_GUI_DIALOG_MIN_HEIGHT).toInt());
}

CDialog::CDialog(int dx, int dy,QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    this->setMinimumSize(SETT.getValue(SETTINGS_GUI_DIALOG_MIN_WIDTH).toInt(), SETT.getValue(SETTINGS_GUI_DIALOG_MIN_HEIGHT).toInt());
    resize(dx, dy);
}

void CDialog::centerWindow()
{
  QWidget *parent = dynamic_cast<QWidget*>(this->parent());
  if (parent)
  {
    int x = parent->x() + (parent->width()/2) - (this->width()/2);
    int y = parent->y() + (parent->height()/2) - (this->height()/2);
    this->move(x,y);
  }
}

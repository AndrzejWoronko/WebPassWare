#include "Dialog.h"

CDialog::CDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{

}

CDialog::CDialog(int dx, int dy,QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
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

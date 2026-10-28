#include "MessageBox.h"

namespace wpw {

void CMessageBox::OkDialogCritical(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Critical, title, text, CMessageBox::Ok, parent);
  messageBox.setDefaultButton(CMessageBox::No);
  messageBox.exec();
  return;
}

void CMessageBox::OkDialogInformation(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Information, title, text, CMessageBox::Ok, parent);
  messageBox.setDefaultButton(CMessageBox::No);
  messageBox.exec();
  return;
}

void CMessageBox::OkDialogWarning(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Warning, title, text, CMessageBox::Ok, parent);
  messageBox.setDefaultButton(CMessageBox::No);
  messageBox.exec();
  return;
}

CMessageBox::StandardButton	CMessageBox::YesNoDialog( const QString & text,  QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Question, title, text, (CMessageBox::Yes | CMessageBox::No), parent);
  messageBox.setDefaultButton(CMessageBox::Yes);
  return (CMessageBox::StandardButton)messageBox.exec();
}

CMessageBox::StandardButton	CMessageBox::NoYesDialog(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Question, title, text, (CMessageBox::Yes | CMessageBox::No), parent);
  messageBox.setDefaultButton(CMessageBox::No);
  return (CMessageBox::StandardButton)messageBox.exec();
}

void CMessageBox::AboutDialog(const QString & text, QWidget * parent, const QString & title)
{
    QMessageBox::about(parent, title, text);
}

void CMessageBox::AboutQtDialog( QWidget * parent, const QString & title)
{
    QMessageBox::aboutQt(parent, title);
}

} // namespace wpw

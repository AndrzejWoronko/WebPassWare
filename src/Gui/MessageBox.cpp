#include "MessageBox.h"

void CMessageBox::OkDialogCritical(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Critical, title, text, CMessageBox::Ok, parent);
  messageBox.setDefaultButton(CMessageBox::No);
  translateButtons(messageBox);
  messageBox.exec();
  return;
}

void CMessageBox::OkDialogInformation(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Information, title, text, CMessageBox::Ok, parent);
  messageBox.setDefaultButton(CMessageBox::No);
  translateButtons(messageBox);
  messageBox.exec();
  return;
}

void CMessageBox::OkDialogWarning(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Warning, title, text, CMessageBox::Ok, parent);
  messageBox.setDefaultButton(CMessageBox::No);
  translateButtons(messageBox);
  messageBox.exec();
  return;
}

CMessageBox::StandardButton	CMessageBox::YesNoDialog( const QString & text,  QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Question, title, text, (CMessageBox::Yes | CMessageBox::No), parent);
  messageBox.setDefaultButton(CMessageBox::Yes);
  translateButtons(messageBox);
  return (CMessageBox::StandardButton)messageBox.exec();
}

CMessageBox::StandardButton	CMessageBox::NoYesDialog(const QString & text, QWidget * parent, const QString & title)
{
  QMessageBox messageBox(QMessageBox::Question, title, text, (CMessageBox::Yes | CMessageBox::No), parent);
  messageBox.setDefaultButton(CMessageBox::No);
  translateButtons(messageBox);
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

void CMessageBox::translateButtons(QMessageBox &messageBox)
{
  Q_FOREACH(auto button, messageBox.buttons())
  {
    if (button->text() == QLatin1String("&Yes"))
      button->setText(QObject::tr("&Yes"));
    if (button->text() == QLatin1String("&No"))
      button->setText(QObject::tr("&No"));
    if (button->text() == QLatin1String("&Reset"))
      button->setText(QObject::tr("&Reset"));
    if (button->text() == QLatin1String("Yes to all"))
      button->setText(QObject::tr("&Yes to all"));
    if (button->text() == QLatin1String("Cancel"))
      button->setText(QObject::tr("&Cancel"));
    if (button->text() == QLatin1String("Save"))
      button->setText(QObject::tr("&Save"));
    if (button->text() == QLatin1String("OK"))
      button->setText(QObject::tr("&Ok"));
  }
}

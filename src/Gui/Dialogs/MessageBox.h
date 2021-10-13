#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "Global.h"
#include <QMessageBox>
#include <QAbstractButton>


class CMessageBox
{
public:

    enum OkDialogType
    {
       Critical = 0,
       Information  = 1,
       Warning = 2
    };

    typedef QMessageBox::StandardButton StandardButton;
    typedef QMessageBox::StandardButtons StandardButtons;

    static const StandardButton No = QMessageBox::No;
    static const StandardButton Yes = QMessageBox::Yes;
    static const StandardButton Ok = QMessageBox::Ok;
    static const StandardButton NoButton = QMessageBox::NoButton;

    static void OkDialogCritical(const QString & text, QWidget  *parent = NULL,
                                 const QString & title = QString(QObject::tr("Błąd krytyczny")));
    static void OkDialogInformation(const QString & text,QWidget  *parent = NULL,
                                    const QString & title = QString(QObject::tr("Informacja")));
    static void OkDialogWarning(const QString & text, QWidget  *parent = NULL,
                                const QString & title = QString(QObject::tr("Uwaga !!!")));
    static StandardButton YesNoDialog(const QString & text, QWidget  *parent = NULL,
                                      const QString & title = QString(QObject::tr("Pytanie")));
    static StandardButton NoYesDialog(const QString & text, QWidget  *parent = NULL,
                                      const QString & title = QString(QObject::tr("Pytanie")));
    static void AboutDialog(const QString & text, QWidget  *parent = NULL,
                            const QString & title = QString(QObject::tr("O programie")));
    static void AboutQtDialog(QWidget  *parent = NULL, const QString & title = QString(QObject::tr("Informacje o QT")));

  private:
    static void translateButtons(QMessageBox &messageBox);
};

#endif // CMESSAGEBOX_H

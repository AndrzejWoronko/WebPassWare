#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "Global.h"
#include "Dialog.h"
#include "AbstractDialogStateManager.h"
#include "ButtonBox.h"
#include "Layouts.h"

class SettingsDialog : public CDialog, public CAbstractDialogStateManager
{
    Q_OBJECT

            /**
      ButtonBox zawierające przyciski potwierdzenia bądź zaprzestania dokonywanej operacji
    */
                ADD_PTR_PROPERTY(CButtonBoxClose, m_buttonBox, getButtonBox)

        public:
                 explicit SettingsDialog(QWidget *w, QWidget *parent = NULL);
    ~SettingsDialog();

    void addFieldsAndComponents();
    virtual void restoreDialogState();
    virtual void saveDialogState();

private:
    CGridLayout *m_formLayout; //layout widget

    QWidget *m_widget; //Widget do pól

    CVBoxLayout *m_VLayoutDialog;  //Do dialogu

signals:

public slots:
};

#endif // SETTINGSDIALOG_H

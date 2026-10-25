#ifndef CLOGINDIALOG_H
#define CLOGINDIALOG_H

#include "Global.h"
#include "Form.h"
#include "Style.h"
#include "Frame.h"
#include "Label.h"
#include "AbstractDialogStateManager.h"

class CDatabase;

class CLoginDialog : public CDialog, public CAbstractDialogStateManager
{
     Q_OBJECT

     ADD_QSMART_SHARED_PTR(CFormTextField, m_passwd, getPasswd)
     // Only on the first run, to confirm the new master password.
     ADD_QSMART_SHARED_PTR(CFormTextField, m_passwd2, getPasswd2)
     ADD_QSMART_SHARED_PTR(CLabel, m_icon_label, getIconLabel)
     ADD_QSMART_SHARED_PTR(CLabel, m_company_label, getCompanyLabel)
     ADD_QSMART_SHARED_PTR(CLabel, m_version_label, getVersionLabel)
     /**
      ButtonBox zawierające przyciski potwierdzenia bądź zaprzestania dokonywanej operacji
    */
     ADD_QSMART_SHARED_PTR(CButtonBoxOk, m_buttonBox, getButtonBox)
public:
     explicit CLoginDialog(CDatabase &db, const QString &title = QString(), const QString &iconName = QString(), QWidget *parent = nullptr);
     ~CLoginDialog();

     virtual void restoreDialogState() override final;
     virtual void saveDialogState() override final;

private:
     void addFields();
     void addFieldsAndComponents();

     QSharedPointer<CGridLayout> m_headerLayout;
     QSharedPointer<CFormLayout> m_bodyLayout;
     QSharedPointer<CHBoxLayout> m_footerLayout;
     CVBoxLayout *m_VLayoutDialog;  //Do dialogu
     CDatabase &m_db;
     // No master password yet: the dialog sets one instead of checking it.
     bool m_firstRun;

private slots:

    virtual void accept() override final;


};

#endif // CLOGINDIALOG_H

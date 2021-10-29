#ifndef CLOGINDIALOG_H
#define CLOGINDIALOG_H

#include "Global.h"
#include "Form.h"
#include "Style.h"
#include "Frame.h"
#include "Label.h"
#include "AbstractDialogStateManager.h"

class CLoginDialog : public CDialog, public CAbstractDialogStateManager
{
     Q_OBJECT

     ADD_PTR_PROPERTY(CFormTextField, m_passwd, getPasswd)
     ADD_PTR_PROPERTY(CLabel, m_icon_label, getIconLabel)
     ADD_PTR_PROPERTY(CLabel, m_company_label, getCompanyLabel)
     ADD_PTR_PROPERTY(CLabel, m_version_label, getVersionLabel)
     /**
      ButtonBox zawierające przyciski potwierdzenia bądź zaprzestania dokonywanej operacji
    */
     ADD_PTR_PROPERTY(CButtonBoxOk, m_buttonBox, getButtonBox)
public:
     CLoginDialog(const QString &title = QString(), const QString &iconName = QString(), QWidget *parent = NULL);
     ~CLoginDialog();

     virtual void restoreDialogState();
     virtual void saveDialogState();

private:
     void addFields();
     void addFieldsAndComponents();

     CGridLayout *m_headerLayout;
     CFormLayout *m_bodyLayout;
     CHBoxLayout *m_footerLayout;
     CVBoxLayout *m_VLayoutDialog;  //Do dialogu

private slots:

     void accept();


};

#endif // CLOGINDIALOG_H

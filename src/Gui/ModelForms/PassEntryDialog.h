#ifndef PASSENTRYDIALOG_H
#define PASSENTRYDIALOG_H

#include "Global.h"
#include "AbstractFormDialog.h"
#include "PasswordGeneratorWidget.h"

class PassEntryDialog : public CAbstractFormDialog
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CFormNumberField, m_id, getId)
    ADD_PTR_PROPERTY(CFormTextField, m_title, getTitle)
    ADD_PTR_PROPERTY(CFormTextField, m_user, getUser)
    ADD_PTR_PROPERTY(CFormTextField, m_pass, getPass)
    ADD_PTR_PROPERTY(CFormTextField, m_web_url, getWebUrl)
    ADD_PTR_PROPERTY(CFormTextField, m_desc, getDec)
    ADD_PTR_PROPERTY(CFormSimpleIndexChoiceField, m_id_pass_group, getIdPassGroup)
    ADD_PTR_PROPERTY(CPasswordGeneratorWidget, m_pass_gen_widget, getPassGenWidget)

public:
    explicit PassEntryDialog(QWidget *parent = NULL);    

    virtual void addFields();
    virtual void addComponents();
};

#endif // PASSENTRYDIALOG_H

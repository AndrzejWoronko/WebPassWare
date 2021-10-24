#ifndef PASSGROUPDIALOG_H
#define PASSGROUPDIALOG_H

#include "Global.h"
#include "AbstractFormDialog.h"

class PassGroupDialog : public CAbstractFormDialog
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CFormNumberField, m_id, getId)
    ADD_PTR_PROPERTY(CFormTextField, m_name, getName)

public:
    explicit PassGroupDialog(QWidget *parent = NULL);

    virtual void addFields();
    virtual void addComponents();
};

#endif // PASSGROUPDIALOG_H

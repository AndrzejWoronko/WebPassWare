#ifndef PASSGROUPDIALOG_H
#define PASSGROUPDIALOG_H

#include "Global.h"
#include "AbstractFormDialog.h"

namespace wpw {

class PassGroupDialog : public CAbstractFormDialog
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CFormNumberField, m_id, getId)
    ADD_PTR_PROPERTY(CFormTextField, m_name, getName)

public:
    explicit PassGroupDialog(QWidget *parent = nullptr);

    virtual void addFields();
    virtual void addComponents();
};

} // namespace wpw

#endif // PASSGROUPDIALOG_H

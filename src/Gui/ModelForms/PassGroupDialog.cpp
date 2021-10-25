#include "PassGroupDialog.h"

PassGroupDialog::PassGroupDialog(QWidget *parent) :
    CAbstractFormDialog(QString("PASS_GROUP_DIALOG"), parent)
{

}

void PassGroupDialog::addFields()
{
    m_id = new CFormNumberField(QString("m_id"), tr("Id"), 0);
    addField(m_id);
    m_name = new CFormTextField(QString("m_name"), tr("Nazwa grupy"), "", 52);
    addField(m_name);
}

void PassGroupDialog::addComponents()
{
    CForm::setComponents(getFormLayout(), m_id, m_id->getLabel(), 0, 1, 1, 1, true);
    CForm::setComponents(getFormLayout(), m_name, m_name->getLabel(), 1, 1, 1, 2);
    getFormLayout()->setColumnStretch(0, 1);
    getFormLayout()->setColumnStretch(1, 1);
    getFormLayout()->setColumnStretch(2, 2);
    setDialogWidget();
}

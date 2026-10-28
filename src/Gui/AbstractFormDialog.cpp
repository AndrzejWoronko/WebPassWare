#include "AbstractFormDialog.h"

namespace wpw {

CAbstractFormDialog::CAbstractFormDialog(const QString &dialog_name, QWidget *parent)
    : CAbstractFieldsDialog(dialog_name, parent), m_buttonBox(nullptr)
{
}

void CAbstractFormDialog::createButtons()
{
    m_buttonBox = new CButtonBox(this);
    dialogLayout()->addWidget(m_buttonBox);
}

void CAbstractFormDialog::closeEvent(QCloseEvent *event)
{
    if (event->type() == QEvent::Close)
    {
//        if (m_controller)
//            m_controller->checkChanges();
        event->accept();
    }
}

FormFieldHashList &CAbstractFormDialog::getFields()
{
    return m_fields;
}

void CAbstractFormDialog::addField(CFormAbstractField *f)
{
    m_fields.insert(f->getVariableName(), f);
}

} // namespace wpw

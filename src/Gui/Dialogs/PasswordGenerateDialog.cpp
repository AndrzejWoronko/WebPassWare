#include "PasswordGenerateDialog.h"

CPasswordGenerateDialog::CPasswordGenerateDialog(QWidget *parent) : CAbstractCloseDialog(QString("PASS_GEN_DIALOG"), parent)
{
    this->setWindowTitle(tr("Generator haseł"));
    this->addFieldsAndComponents();
}

CPasswordGenerateDialog::~CPasswordGenerateDialog()
{
    safe_delete(m_pass_gen_widget)
}

void CPasswordGenerateDialog::addFields()
{
    m_pass_gen_widget =  new CPasswordGeneratorWidget(this);
}

void CPasswordGenerateDialog::addComponents()
{
    CForm::setComponents(getFormLayout(), m_pass_gen_widget, QString(), 0, 0, 6, 8);
    m_pass_gen_widget->getAcceptButton()->setVisible(false);
    setDialogWidget();
}

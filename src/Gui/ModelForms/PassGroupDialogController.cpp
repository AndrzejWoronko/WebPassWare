#include "PassGroupDialogController.h"

PassGroupDialogController::PassGroupDialogController(QWidget *parent) :
    CAbstractFormDialogController(new PassGroupDialog(parent), parent)
{
    if (m_dialog)
    {
        m_dialog->addFieldsAndComponents();
        this->setConnections();
        this->restoreDialogState();
        m_validator = new CFormValidator(m_dialog->getFields());
        m_validator->setValidateCallback(validateForm);
        m_validator->setValidateEmpty(isEmptyForm);
    }
}

PassGroupDialogController::~PassGroupDialogController()
{
    if (m_dialog)
        this->saveDialogState();
}

QString PassGroupDialogController::isEmptyForm(FormFieldHashList fields)
{
    QString error = QString("");

    CFormAbstractField *f;
    f = fields.value("m_name");

    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    return error;
}

QString PassGroupDialogController::validateForm(FormFieldHashList fields)
{
    QString error = QString("");

    CFormAbstractField *f;

    f = fields.value("m_name");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    return error;
}

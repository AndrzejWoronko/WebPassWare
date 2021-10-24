#include "PassEntryDialogController.h"

PassEntryDialogController::PassEntryDialogController(QWidget *parent) :
    CAbstractFormDialogController(new PassEntryDialog(parent), parent)
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

PassEntryDialogController::~PassEntryDialogController()
{
    if (m_dialog)
        this->saveDialogState();
}

QString PassEntryDialogController::isEmptyForm(FormFieldHashList fields)
{
    QString error = QString("");

    CFormAbstractField *f;
    f = fields.value("m_title");

    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    f = fields.value("m_user");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    f = fields.value("m_pass");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    f = fields.value("m_web_url");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    return error;
}

QString PassEntryDialogController::validateForm(FormFieldHashList fields)
{
    QString error = QString("");

    CFormAbstractField *f;

    f = fields.value("m_title");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    f = fields.value("m_user");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    f = fields.value("m_pass");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    f = fields.value("m_web_url");
    if (f && f->getValue().toString().length()==0)
        error += QString("%1: %2.\n").arg(tr("Puste pole")).arg(f->getLabel());

    return error;
}

bool PassEntryDialogController::exec(const QString &title)
{
    if (m_dialog)
    {
        m_dialog->setWindowTitle(title);
        CFormAbstractField *f = m_dialog->getFields().value("m_id");
        if (f)
            f->getWidget()->setVisible(false);

        if (m_dialog->exec())
        {

        }
    }
    return false;
}

bool PassEntryDialogController::exec(qint64 id, const QString &title)
{
    if (m_dialog)
    {
        m_dialog->setWindowTitle(title);
        CFormAbstractField *f = m_dialog->getFields().value("m_id");
        if (f && dynamic_cast<CFormNumberField*>(f))
        {
            dynamic_cast<CFormNumberField*>(f)->setReadOnly();
        }
        if (m_dialog->exec())
        {

        }
    }
    return false;
}

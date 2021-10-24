#include "PassGroupDialogController.h"
#include "PassGroupService.h"

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

bool PassGroupDialogController::exec(const QString &title)
{
    if (m_dialog)
    {
        m_dialog->setWindowTitle(title);
        CFormAbstractField *f = m_dialog->getFields().value("m_id");
        if (f)
            f->getWidget()->setVisible(false);

        if (m_dialog->exec())
        {
            PassGroup pg;
            f = m_dialog->getFields().value("m_name");
            if (f)
                pg.setm_name(f->getValue().toString());

            return PassGroupService::getInstance().addObject(&pg) != -1;
        }
    }
    return false;
}

bool PassGroupDialogController::exec(qint64 id, const QString &title)
{
    bool ret = false;
    if (m_dialog)
    {
        m_dialog->setWindowTitle(title);
        CFormAbstractField *f = m_dialog->getFields().value("m_id");
        if (f && dynamic_cast<CFormNumberField*>(f))
           {
             dynamic_cast<CFormNumberField*>(f)->setReadOnly();
           }
        PassGroup *pg = PassGroupService::getInstance().getObject(id);
        if (pg)
           {
                f = m_dialog->getFields().value("id");
                if (f)
                    f->setValue(pg->getId());
                f = m_dialog->getFields().value("m_name");
                if (f)
                    f->setValue(pg->getm_name());
           }
        if (m_dialog->exec())
           {
               if (pg)
               {
                   f = m_dialog->getFields().value("m_name");
                   if (f)
                       pg->setm_name(f->getValue().toString());
                   ret = PassGroupService::getInstance().editObject(pg);
               }
           }
       safe_delete(pg)
    }
    return ret;
}

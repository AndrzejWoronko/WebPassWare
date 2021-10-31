#include "PassEntryDialogController.h"
#include "PassEntryService.h"
#include "PassGroupService.h"

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
        CFormAbstractField *f;
        f = m_dialog->getFields().value("m_id_pass_group");
        if (f && dynamic_cast<CFormSimpleIndexChoiceField*>(f))
        {
            dynamic_cast<CFormSimpleIndexChoiceField*>(f)->setValueList(PassGroupService::getInstance().getGroupNameList());
        }
        auto dlg = dynamic_cast<PassEntryDialog*>(m_dialog);
        if (dlg)
        {
            connect(dlg->getPassGenWidget(), SIGNAL(newPassword(const QString&)), this, SLOT(setGenPassword(const QString&)));
        }
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
          CFormAbstractField *f = NULL;
//        f = m_dialog->getFields().value("m_id");
//        if (f)
//            f->getWidget()->setVisible(false);

        if (m_dialog->exec())
        {
            PassEntry pe;
            //Podstawienie wartości z formatki
            f = m_dialog->getFields().value("m_title");
            if (f)
                pe.setm_title(f->getValue().toString());
            f = m_dialog->getFields().value("m_user");
            if (f)
                pe.setm_user(f->getValue().toString());
            f = m_dialog->getFields().value("m_pass");
            if (f)
                pe.setm_pass(f->getValue().toString());
            f = m_dialog->getFields().value("m_web_url");
            if (f)
                pe.setm_web_url(f->getValue().toString());
            f = m_dialog->getFields().value("m_desc");
            if (f)
                pe.setm_desc(f->getValue().toString());
            f = m_dialog->getFields().value("m_id_pass_group");
            if (f && dynamic_cast<CFormSimpleIndexChoiceField*>(f))
            {
                pe.setm_id_pass_group(dynamic_cast<CFormSimpleIndexChoiceField*>(f)->getIndexValue() + 1);
            }

            return PassEntryService::getInstance().addObject(&pe) != -1; //Zapis do bazy
        }
    }
    return false;
}

bool PassEntryDialogController::exec(qint64 id, const QString &title)
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
        PassEntry *pe = PassEntryService::getInstance().getObject(id);
        if (pe) //Podtawienie wartości do formatki
        {
            f = m_dialog->getFields().value("m_id");
            if (f)
                f->setValue(pe->getId());
            f = m_dialog->getFields().value("m_title");
            if (f)
                f->setValue(pe->getm_title());
            f = m_dialog->getFields().value("m_user");
            if (f)
                f->setValue(pe->getm_user());
            f = m_dialog->getFields().value("m_pass");
            if (f)
                f->setValue(pe->getm_pass());
            f = m_dialog->getFields().value("m_web_url");
            if (f)
                f->setValue(pe->getm_web_url());
            f = m_dialog->getFields().value("m_desc");
            if (f)
                f->setValue(pe->getm_desc());
            f = m_dialog->getFields().value("m_id_pass_group");
            if (f && dynamic_cast<CFormSimpleIndexChoiceField*>(f))
            {
                dynamic_cast<CFormSimpleIndexChoiceField*>(f)->setIndexValue(pe->getm_id_pass_group() - 1);
//                if (f)
//                    f->setValue(pe->getm_id_pass_group());
            }
        }

        if (m_dialog->exec())
        {
            if (pe)
            {
                f = m_dialog->getFields().value("m_title");
                if (f)
                    pe->setm_title(f->getValue().toString());
                f = m_dialog->getFields().value("m_user");
                if (f)
                    pe->setm_user(f->getValue().toString());
                f = m_dialog->getFields().value("m_pass");
                if (f)
                    pe->setm_pass(f->getValue().toString());
                f = m_dialog->getFields().value("m_web_url");
                if (f)
                    pe->setm_web_url(f->getValue().toString());
                f = m_dialog->getFields().value("m_desc");
                if (f)
                    pe->setm_desc(f->getValue().toString());
                f = m_dialog->getFields().value("m_id_pass_group");
                if (f && dynamic_cast<CFormSimpleIndexChoiceField*>(f))
                {
                    pe->setm_id_pass_group(dynamic_cast<CFormSimpleIndexChoiceField*>(f)->getIndexValue() + 1);
                }
                ret = PassEntryService::getInstance().editObject(pe);
            }
        }
        safe_delete(pe)
    }
    return ret;
}

void PassEntryDialogController::setGenPassword(const QString& password)
{
    CFormAbstractField *f = m_dialog->getFields().value("m_pass");
    if (f)
        f->setValue(password);
}

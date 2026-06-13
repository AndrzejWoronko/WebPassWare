#include "PassEntryDialogController.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "MessageBox.h"

PassEntryDialogController::PassEntryDialogController(QWidget *parent) :
    PassEntryDialogController(&PassEntryService::getInstance(), &PassGroupService::getInstance(), parent)
{
}

PassEntryDialogController::PassEntryDialogController(PassEntryService *passEntryService, PassGroupService *passGroupService, QWidget *parent) :
    CAbstractFormDialogController(new PassEntryDialog(parent), parent),
    m_passEntryService(passEntryService ? passEntryService : &PassEntryService::getInstance()),
    m_passGroupService(passGroupService ? passGroupService : &PassGroupService::getInstance())
{
    if (m_dialog)
    {
        m_dialog->addFieldsAndComponents();
        this->setConnections();
        this->restoreDialogState();
        m_validator = new CFormValidator(m_dialog->getFields());
        m_validator->setValidateCallback(validateForm);
        m_validator->setValidateEmpty(isEmptyForm);
        CFormSimpleIndexChoiceField *groupField = getFieldAs<CFormSimpleIndexChoiceField>("m_id_pass_group");
        if (groupField)
        {
            groupField->setValueList(m_passGroupService->getGroupNameList());
        }
        auto dlg = getDialogAs<PassEntryDialog>();
        if (dlg)
        {
            connect(dlg->getPassGenWidget(), &CPasswordGeneratorWidget::newPassword, this, &PassEntryDialogController::setGenPassword);
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
          CFormAbstractField *f = nullptr;
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
            CFormSimpleIndexChoiceField *groupField = getFieldAs<CFormSimpleIndexChoiceField>("m_id_pass_group");
            if (groupField)
            {
                if (!m_passGroupService->getOwnedObject(0).isNull())
                {
                    pe.setm_id_pass_group(groupField->getIndexValue());
                }
                else
                {
                    pe.setm_id_pass_group(groupField->getIndexValue() + 1);
                }
            }

            const qint64 newId = m_passEntryService->addObject(&pe); //Zapis do bazy
            if (newId < 0)
            {
                CMessageBox::OkDialogWarning(QString("%1\n%2: %3").arg(tr("Błąd dodawania rekordu !!!"), tr("Opis błędu"), m_passEntryService->getError()), this);
                return false;
            }
            return true;
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
        CFormNumberField *idField = getFieldAs<CFormNumberField>("m_id");
        if (idField)
        {
            idField->setReadOnly();
        }
        PassEntryService::OwnedObject pe = m_passEntryService->getOwnedObject(id);
        if (pe.isNull())
        {
            QString error = m_passEntryService->getError();
            if (error.isEmpty())
            {
                error = tr("Nie znaleziono rekordu o id: %1.").arg(id);
            }
            CMessageBox::OkDialogWarning(QString("%1\n%2: %3").arg(tr("Błąd wczytywania rekordu !!!"), tr("Opis błędu"), error), this);
            return false;
        }
        //Podtawienie wartości do formatki
        {
            CFormAbstractField *f = m_dialog->getFields().value("m_id");
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
            CFormSimpleIndexChoiceField *groupField = getFieldAs<CFormSimpleIndexChoiceField>("m_id_pass_group");
            if (groupField)
            {
                if (!m_passGroupService->getOwnedObject(0).isNull())
                {
                    groupField->setIndexValue(pe->getm_id_pass_group());
                }
                else
                {
                    groupField->setIndexValue(pe->getm_id_pass_group() - 1);
                }
//                if (f)
//                    f->setValue(pe->getm_id_pass_group());
            }
        }

        if (m_dialog->exec())
        {
            CFormAbstractField *f = m_dialog->getFields().value("m_title");
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
            CFormSimpleIndexChoiceField *groupField = getFieldAs<CFormSimpleIndexChoiceField>("m_id_pass_group");
            if (groupField)
            {
                if (!m_passGroupService->getOwnedObject(0).isNull())
                {
                    pe->setm_id_pass_group(groupField->getIndexValue());
                }
                else
                {
                    pe->setm_id_pass_group(groupField->getIndexValue() + 1);
                }
            }
            ret = m_passEntryService->editObject(pe.data());
            if (!ret)
            {
                CMessageBox::OkDialogWarning(QString("%1\n%2: %3").arg(tr("Błąd edycji rekordu !!!"), tr("Opis błędu"), m_passEntryService->getError()), this);
            }
        }
    }
    return ret;
}

void PassEntryDialogController::setGenPassword(const QString& password)
{
    CFormAbstractField *f = m_dialog->getFields().value("m_pass");
    if (f)
        f->setValue(password);
}

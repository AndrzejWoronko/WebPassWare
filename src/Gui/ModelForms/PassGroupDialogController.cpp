#include "PassGroupDialogController.h"
#include "PassGroupService.h"
#include "MessageBox.h"

namespace wpw {

PassGroupDialogController::PassGroupDialogController(PassGroupService *passGroupService, QWidget *parent) :
    CAbstractFormDialogController(new PassGroupDialog(parent), parent),
    m_passGroupService(passGroupService)
{
    if (m_dialog)
    {
        m_dialog->addFieldsAndComponents();
        this->setConnections();
        this->restoreDialogState();
        m_validator = new CFormValidator(m_dialog->getFields());
        // An empty required field is both "empty" and "invalid".
        m_validator->setValidateCallback(requiredFieldsError);
        m_validator->setValidateEmpty(requiredFieldsError);
    }
}

PassGroupDialogController::~PassGroupDialogController()
{
    if (m_dialog)
        this->saveDialogState();
}

QString PassGroupDialogController::requiredFieldsError(FormFieldHashList fields)
{
    return emptyRequiredFields(fields, {"m_name"});
}

bool PassGroupDialogController::exec(const QString &title)
{
    if (m_dialog)
    {
        m_dialog->setWindowTitle(title);
        CFormAbstractField *f = nullptr;
//        m_dialog->getFields().value("m_id");
//        if (f)
//            f->getWidget()->setVisible(false);

        if (m_dialog->exec())
        {
            PassGroup pg;
            f = m_dialog->getFields().value("m_name");
            if (f)
                pg.setm_name(f->getValue().toString());

            const qint64 newId = m_passGroupService->addObject(&pg);
            if (newId < 0)
            {
                CMessageBox::OkDialogWarning(QString("%1\n%2: %3").arg(tr("Błąd dodawania grupy !!!"), tr("Opis błędu"), m_passGroupService->getError()), m_dialog);
                return false;
            }
            return true;
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
        CFormNumberField *idField = getFieldAs<CFormNumberField>("m_id");
        if (idField)
        {
            idField->setReadOnly();
        }
        PassGroupService::OwnedObject pg = m_passGroupService->getOwnedObject(id);
        if (pg.isNull())
        {
            QString error = m_passGroupService->getError();
            if (error.isEmpty())
            {
                error = tr("Nie znaleziono grupy o id: %1.").arg(id);
            }
            CMessageBox::OkDialogWarning(QString("%1\n%2: %3").arg(tr("Błąd wczytywania grupy !!!"), tr("Opis błędu"), error), m_dialog);
            return false;
        }
        CFormAbstractField *f = m_dialog->getFields().value("m_id");
        if (f)
            f->setValue(pg->getId());
        f = m_dialog->getFields().value("m_name");
        if (f)
            f->setValue(pg->getm_name());
        if (m_dialog->exec())
           {
               f = m_dialog->getFields().value("m_name");
               if (f)
                   pg->setm_name(f->getValue().toString());
               ret = m_passGroupService->editObject(pg.data());
               if (!ret)
               {
                   CMessageBox::OkDialogWarning(QString("%1\n%2: %3").arg(tr("Błąd edycji grupy !!!"), tr("Opis błędu"), m_passGroupService->getError()), m_dialog);
               }
           }
    }
    return ret;
}

} // namespace wpw

#include "AbstractFormDialogController.h"
#include "MessageBox.h"

CAbstractFormDialogController::CAbstractFormDialogController(CAbstractFormDialog *dialog, QWidget *parent) :
    QWidget(parent), CAbstractDialogStateManager()
{
    m_dialog = dialog;
}

//Zapisanie i odtworzenie układu graficznego dialogu

void CAbstractFormDialogController::restoreDialogState()
{
    QByteArray state = m_dialogState->getState(m_dialog->getDialogName());
    m_dialog->restoreGeometry(state);
}

void CAbstractFormDialogController::saveDialogState()
{
    QByteArray state = m_dialog->saveGeometry();
    QByteArray oldState = m_dialogState->getState(m_dialog->getDialogName());

    if(state != oldState)
        m_dialogState->saveState(m_dialog->getDialogName(), state);
}

void CAbstractFormDialogController::setConnections()
{
    if (m_dialog)
    {
        connect(m_dialog->getButtonBox(), SIGNAL(accepted()), this, SLOT(checkRequiredFields()));
        connect(m_dialog->getButtonBox(), SIGNAL(rejected()), this, SLOT(checkChanges()));
    }
}

CAbstractFormDialog *CAbstractFormDialogController::getDialog()
{
    return m_dialog;
}

bool CAbstractFormDialogController::isChangesOccured()
{
    Q_FOREACH(auto field, m_dialog->getFields().values())
    {
        if (field && field->isDirty())
        {
            DEBUG_WITH_LINE << "Field: " << field->getVariableName() <<  " value: " << field->getValue() << "start value: " << field->getStartValue();
            return true;
        }
    }
    return false;
/*
    for (int i = 0; i < m_dialog->getFields().count(); i++)
    {
        CFormAbstractField *field = m_dialog->getFields().values()[i];
          if (field && field->isDirty())
            {
                DEBUG_WITH_LINE << "Field: " << field->getVariableName() <<  " value: " << field->getValue() << "start value: " << field->getStartValue();
                return true;
            }
    }
    return false;
*/
}

void CAbstractFormDialogController::checkRequiredFields()
{
    if (m_validator->isEmptyForm())
       {
            CMessageBox::OkDialogWarning(tr("Uzupełnij / popraw dane !!!"), m_dialog, tr("Uzupełnij / popraw dane !!!"));
       }
    else
        {
           QString error = m_validator->validateForm();
           if (error.length() == 0) //Brak błędów walidacji
              {
               m_dialog->accept();
              }
           else
              {
                 CMessageBox::OkDialogWarning(error, m_dialog, tr("Uzupełnij / popraw dane !!!"));
              }
        }
}

void CAbstractFormDialogController::checkChanges()
{
    if(m_validator->isEmptyForm())
       {
          m_dialog->reject();
       }
    else if(isChangesOccured())
       {
           if (CMessageBox::YesNoDialog(tr("Czy chcesz zapisać zmiany ?"), m_dialog, tr("Dokonano zmian")) == (CMessageBox::Yes))
               {
                   this->checkRequiredFields();
               }
           else
               m_dialog->reject(); // zmiany dokonane, ale użytkowik chce anulować
       }
       else
           m_dialog->reject(); // nie dokonano zmian, anuluj
}


QJsonObject CAbstractFormDialogController::toJson()
{
    QJsonObject jo;
    Q_FOREACH(auto key, m_dialog->getFields().keys())
    {
        if (m_dialog->getFields().value(key))
            jo.insert(key, QJsonValue::fromVariant(m_dialog->getFields().value(key)->getValue()));
    }
    return jo;
}

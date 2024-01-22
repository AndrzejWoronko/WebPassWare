#include "DialogState.h"
#include "Application.h"

CDialogState::CDialogState()
{
    APPI->setAppInformation();
    m_settings.reset(new QSettings());
    m_settings->beginGroup("DialogStates");
}

CDialogState::~CDialogState()
{
    m_settings->endGroup();
    DEBUG_WITH_LINE << "QScopedPointer destructor: " << getSettings()->fileName();
    //safe_delete(m_settings)
}

void CDialogState::saveState(const QString &dialogName, QByteArray state)
{
    m_settings->setValue(dialogName, state);
}

QByteArray CDialogState::getState(const QString &dialogName)
{
    return  m_settings->value(dialogName).toByteArray();
}
/*
std::unique_ptr<QSettings>& CDialogState::getSettings()
{
    return m_settings;
}
*/

#include "TabMainWindowState.h"
#include "Application.h"

CTabMainWindowState::CTabMainWindowState()
{
    APPI->setAppInformation();
    m_settings = new QSettings();
    m_settings->beginGroup("TabMainWindowStates");
}

CTabMainWindowState::~CTabMainWindowState()
{
    m_settings->endGroup();
    safe_delete(m_settings)
}

void CTabMainWindowState::saveState(const QString &tabMainWindowName, QByteArray state)
{
    m_settings->setValue(tabMainWindowName, state);
}

QByteArray CTabMainWindowState::getState(const QString &tabMainWindowName)
{
    return m_settings->value(tabMainWindowName).toByteArray();
}

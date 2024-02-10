#include "TabMainWindowState.h"
#include "Application.h"

CTabMainWindowState::CTabMainWindowState()
{
    //APPI->setAppInformation();
    m_settings.reset(new QSettings());
    m_settings->beginGroup("TabMainWindowStates");
}

CTabMainWindowState::~CTabMainWindowState()
{
    m_settings->endGroup();
    DEBUG_WITH_LINE << "QScopedPointer ~dtor: " << getSettings()->fileName();
    //safe_delete(m_settings)
}

void CTabMainWindowState::saveState(const QString &tabMainWindowName, const QByteArray &state)
{
    m_settings->setValue(tabMainWindowName, state);
}

QByteArray CTabMainWindowState::getState(const QString &tabMainWindowName)
{
    return m_settings->value(tabMainWindowName).toByteArray();
}

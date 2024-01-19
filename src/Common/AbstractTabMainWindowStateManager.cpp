#include "AbstractTabMainWindowStateManager.h"

CAbstractTabMainWindowStateManager::CAbstractTabMainWindowStateManager()
{
    m_tabMainWindowState = std::make_unique<CTabMainWindowState>();
}

CAbstractTabMainWindowStateManager::~CAbstractTabMainWindowStateManager()
{
    //safe_delete(m_tabMainWindowState)
}

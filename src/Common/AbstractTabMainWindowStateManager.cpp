#include "AbstractTabMainWindowStateManager.h"

CAbstractTabMainWindowStateManager::CAbstractTabMainWindowStateManager()
{
    m_tabMainWindowState = new CTabMainWindowState();
}

CAbstractTabMainWindowStateManager::~CAbstractTabMainWindowStateManager()
{
    safe_delete(m_tabMainWindowState)
}

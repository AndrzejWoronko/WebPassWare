#include "AbstractDialogStateManager.h"

CAbstractDialogStateManager::CAbstractDialogStateManager()
{
    m_dialogState = new CDialogState();
}

CAbstractDialogStateManager::~CAbstractDialogStateManager()
{
    safe_delete(m_dialogState)
}

#include "AbstractTableStateManager.h"

CAbstractTableStateManager::CAbstractTableStateManager()
{
    m_tableState = new CTableState();
}

CAbstractTableStateManager::~CAbstractTableStateManager()
{
    safe_delete(m_tableState)
}

#include "AbstractTableStateManager.h"

CAbstractTableStateManager::CAbstractTableStateManager()
{
    m_tableState = std::make_unique<CTableState>();
}

CAbstractTableStateManager::~CAbstractTableStateManager()
{
    //safe_delete(m_tableState)
}

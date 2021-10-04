#include "AbstractSplitterStateManager.h"

CAbstractSplitterStateManager::CAbstractSplitterStateManager()
{
    m_splitterState = new CSplitterState();
}

CAbstractSplitterStateManager::~CAbstractSplitterStateManager()
{
    safe_delete(m_splitterState)
}

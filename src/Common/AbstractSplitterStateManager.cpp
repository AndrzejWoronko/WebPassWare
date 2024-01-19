#include "AbstractSplitterStateManager.h"

CAbstractSplitterStateManager::CAbstractSplitterStateManager()
{
    m_splitterState = std::make_unique<CSplitterState>();
}

CAbstractSplitterStateManager::~CAbstractSplitterStateManager()
{
    //safe_delete(m_splitterState)
}

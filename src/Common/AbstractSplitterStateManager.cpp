#include "AbstractSplitterStateManager.h"

CAbstractSplitterStateManager::CAbstractSplitterStateManager()
{
    m_splitterState.reset(new CSplitterState());
}

/*
CAbstractSplitterStateManager::~CAbstractSplitterStateManager()
{
    //safe_delete(m_splitterState)
}
*/

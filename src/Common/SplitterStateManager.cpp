#include "SplitterStateManager.h"

CSplitterStateManager::CSplitterStateManager()
{
    m_splitterState.reset(new CSplitterState());
}

void CSplitterStateManager::getState(const QString &splitterName, QByteArray& state)
{
    if (m_splitterState)
        state = m_splitterState->getState(splitterName);
}

void CSplitterStateManager::saveState(const QString &splitterName, const QByteArray &state)
{
    if (m_splitterState)
        m_splitterState->saveState(splitterName, state);
}

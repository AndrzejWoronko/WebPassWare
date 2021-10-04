#include "SplitterState.h"
#include "Application.h"

CSplitterState::CSplitterState()
{
    APPI->setAppInformation();
    m_settings = new QSettings();
    m_settings->beginGroup("SplitterStates");
}

CSplitterState::~CSplitterState()
{
    m_settings->endGroup();
    safe_delete(m_settings)
}

void CSplitterState::saveState(const QString &splitterName, QByteArray state)
{    
    m_settings->setValue(splitterName, state);
}

QByteArray CSplitterState::getState(const QString &splitterName)
{
    return m_settings->value(splitterName).toByteArray();
}

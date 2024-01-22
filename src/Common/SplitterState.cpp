#include "SplitterState.h"
#include "Application.h"

CSplitterState::CSplitterState()
{
    APPI->setAppInformation();
    m_settings = std::make_shared<QSettings>();
    m_settings->beginGroup("SplitterStates");
}

CSplitterState::~CSplitterState()
{
    m_settings->endGroup();
    DEBUG_WITH_LINE << "make_shared dtor: " << getSettings()->fileName();
    //safe_delete(m_settings)
}

void CSplitterState::saveState(const QString &splitterName, QByteArray state)
{    
    m_settings->setValue(splitterName, state);
}

QByteArray CSplitterState::getState(const QString &splitterName)
{
    return m_settings->value(splitterName).toByteArray();
}

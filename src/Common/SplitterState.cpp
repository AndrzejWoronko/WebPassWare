#include "SplitterState.h"
#include "Application.h"

CSplitterState::CSplitterState()
{
    //APPI->setAppInformation();
    m_settings.reset(new QSettings());
    m_settings->beginGroup("SplitterStates");
}

CSplitterState::~CSplitterState()
{
    m_settings->endGroup();
    DEBUG_WITH_LINE << "QScopedPointer ~dtor: " << getSettings()->fileName();
}

void CSplitterState::saveState(const QString &splitterName, const QByteArray &state)
{    
    m_settings->setValue(splitterName, state);
}

QByteArray CSplitterState::getState(const QString &splitterName)
{
    return m_settings->value(splitterName).toByteArray();
}

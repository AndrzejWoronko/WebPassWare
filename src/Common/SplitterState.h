#ifndef CSPLITTERSTATE_H
#define CSPLITTERSTATE_H

#include "Global.h"
#include "AbstractStateManager.h"

class CSplitterState : public ICState
{
    ADD_QSMART_UNIQUE_PTR(QSettings, m_settings, getSettings)

public:
    CSplitterState();
    ~CSplitterState();

    virtual void saveState(const QString &splitterName, const QByteArray &state) override final;
    virtual QByteArray getState(const QString &splitterName) override final;

};

#endif // CSPLITTERSTATE_H

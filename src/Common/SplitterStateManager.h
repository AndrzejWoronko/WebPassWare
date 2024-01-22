#ifndef SPLITTERSTATEMANAGER_H
#define SPLITTERSTATEMANAGER_H

#include "Global.h"
#include "AbstractStateManager.h"
#include "SplitterState.h"

class CSplitterStateManager : public ICStateManager
{
    QScopedPointer<CSplitterState> m_splitterState;

public:
    CSplitterStateManager();
    ~CSplitterStateManager() = default;

    virtual void getState(const QString &splitterName, QByteArray& state) override final;
    virtual void saveState(const QString &splitterName, const QByteArray &state) override final;
};


#endif // SPLITTERSTATEMANAGER_H

#ifndef CTABMAINWINDOWSTATE_H
#define CTABMAINWINDOWSTATE_H

#include "Global.h"
#include "AbstractStateManager.h"

class CTabMainWindowState: public ICState
{

    ADD_QSMART_UNIQUE_PTR(QSettings, m_settings, getSettings)

public:
    CTabMainWindowState();
    ~CTabMainWindowState();

    virtual void saveState(const QString &tabMainWindowName, const QByteArray &state) override final;
    virtual QByteArray getState(const QString &tabMainWindowName) override final;

};


#endif // CTABMAINWINDOWSTATE_H

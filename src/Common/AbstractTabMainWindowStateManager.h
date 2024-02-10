#ifndef CABSTRACTTABMAINWINDOWSTATEMANAGER_H
#define CABSTRACTTABMAINWINDOWSTATEMANAGER_H

#include "TabMainWindowState.h"

class ICTabMainWindowStateManager
{

public:
    ICTabMainWindowStateManager() = default;
    virtual ~ICTabMainWindowStateManager() = default;

    virtual void restoreTabMainWindowState() = 0;
    virtual void saveTabMainWindowState() = 0;
};

class CAbstractTabMainWindowStateManager : public ICTabMainWindowStateManager
{

    ADD_QSMART_UNIQUE_PTR(ICState, m_tabMainWindowState, getTabMainWindowState)

public:
    CAbstractTabMainWindowStateManager();
    ~CAbstractTabMainWindowStateManager() = default;
};


#endif // CABSTRACTTABMAINWINDOWSTATEMANAGER_H

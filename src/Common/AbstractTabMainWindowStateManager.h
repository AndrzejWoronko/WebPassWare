#ifndef CABSTRACTTABMAINWINDOWSTATEMANAGER_H
#define CABSTRACTTABMAINWINDOWSTATEMANAGER_H

#include "TabMainWindowState.h"

class CAbstractTabMainWindowStateManager
{

    ADD_QSMART_PTR(ICState, m_tabMainWindowState, getTabMainWindowState)

public:
    CAbstractTabMainWindowStateManager();
    ~CAbstractTabMainWindowStateManager() = default;

    virtual void restoreTabMainWindowState() = 0;
    virtual void saveTabMainWindowState() = 0;
};


#endif // CABSTRACTTABMAINWINDOWSTATEMANAGER_H

#ifndef CABSTRACTTABMAINWINDOWSTATEMANAGER_H
#define CABSTRACTTABMAINWINDOWSTATEMANAGER_H

#include "TabMainWindowState.h"

class CAbstractTabMainWindowStateManager
{
protected:
    std::unique_ptr<CTabMainWindowState> m_tabMainWindowState;

    virtual void restoreTabMainWindowState() = 0;
    virtual void saveTabMainWindowState() = 0;
public:
    CAbstractTabMainWindowStateManager();
    ~CAbstractTabMainWindowStateManager() = default;
};


#endif // CABSTRACTTABMAINWINDOWSTATEMANAGER_H

#ifndef ABSTRACTTABLESTATEMANAGER_H
#define ABSTRACTTABLESTATEMANAGER_H

#include "TableState.h"

class CAbstractTableStateManager
{
protected:
    std::unique_ptr<CTableState> m_tableState;

    virtual void restoreTableState() = 0;
    virtual void saveTableState() = 0;
public:
    CAbstractTableStateManager();
    ~CAbstractTableStateManager() = default;
};

#endif // ABSTRACTTABLESTATEMANAGER_H

#ifndef ABSTRACTTABLESTATEMANAGER_H
#define ABSTRACTTABLESTATEMANAGER_H

#include "TableState.h"

class CAbstractTableStateManager
{

    ADD_QSMART_PTR(CTableState, m_tableState, getTableState)

public:
    CAbstractTableStateManager();
    ~CAbstractTableStateManager() = default;

    virtual void restoreTableState() = 0;
    virtual void saveTableState() = 0;
};

#endif // ABSTRACTTABLESTATEMANAGER_H

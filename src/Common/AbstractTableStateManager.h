#ifndef ABSTRACTTABLESTATEMANAGER_H
#define ABSTRACTTABLESTATEMANAGER_H

#include "TableState.h"


class ICTableStateManager
{
public:

    ICTableStateManager() = default;
    ~ICTableStateManager() = default;

    virtual void restoreTableState() = 0;
    virtual void saveTableState() = 0;
};


class CAbstractTableStateManager : public ICTableStateManager
{

    ADD_QSMART_UNIQUE_PTR(ICState, m_tableState, getTableState)

public:
    CAbstractTableStateManager();
    ~CAbstractTableStateManager() = default;
};

#endif // ABSTRACTTABLESTATEMANAGER_H

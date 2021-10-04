#ifndef ABSTRACTTABLESTATEMANAGER_H
#define ABSTRACTTABLESTATEMANAGER_H

#include "TableState.h"

class CAbstractTableStateManager
{
protected:
    CTableState *m_tableState;

    virtual void restoreTableState() = 0;
    virtual void saveTableState() = 0;
public:
    CAbstractTableStateManager();
    ~CAbstractTableStateManager();
};

#endif // ABSTRACTTABLESTATEMANAGER_H

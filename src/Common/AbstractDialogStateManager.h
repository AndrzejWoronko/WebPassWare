#ifndef ABSTRACTDIALOGSTATEMANAGER_H
#define ABSTRACTDIALOGSTATEMANAGER_H

#include "DialogState.h"


class ICDialogStateManager
{
public:
    ICDialogStateManager() = default;
    virtual ~ICDialogStateManager() = default;

    virtual void restoreDialogState() = 0;
    virtual void saveDialogState() = 0;
};

class CAbstractDialogStateManager : public ICDialogStateManager
{

    ADD_QSMART_UNIQUE_PTR(ICState, m_dialogState, getDialogState)

public:
    CAbstractDialogStateManager();
    ~CAbstractDialogStateManager() = default;
};

#endif // ABSTRACTDIALOGSTATEMANAGER_H

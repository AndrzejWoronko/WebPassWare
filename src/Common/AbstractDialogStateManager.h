#ifndef ABSTRACTDIALOGSTATEMANAGER_H
#define ABSTRACTDIALOGSTATEMANAGER_H

#include "DialogState.h"

class CAbstractDialogStateManager
{

    ADD_QSMART_PTR(ICState, m_dialogState, getDialogState)

public:
    CAbstractDialogStateManager();
    ~CAbstractDialogStateManager() = default;

    virtual void restoreDialogState() = 0;
    virtual void saveDialogState() = 0;
};

#endif // ABSTRACTDIALOGSTATEMANAGER_H

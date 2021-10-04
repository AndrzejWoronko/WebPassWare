#ifndef ABSTRACTDIALOGSTATEMANAGER_H
#define ABSTRACTDIALOGSTATEMANAGER_H

#include "DialogState.h"

class CAbstractDialogStateManager
{

protected:
    CDialogState *m_dialogState;

public:
    CAbstractDialogStateManager();
    ~CAbstractDialogStateManager();

    virtual void restoreDialogState() = 0;
    virtual void saveDialogState() = 0;
};

#endif // ABSTRACTDIALOGSTATEMANAGER_H

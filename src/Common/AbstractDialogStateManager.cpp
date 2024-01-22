#include "AbstractDialogStateManager.h"
#include <memory>

CAbstractDialogStateManager::CAbstractDialogStateManager()
{
    //m_dialogState = new CDialogState();
    //std::unique_ptr<CDialogState> dialog_state = std::make_unique<CDialogState>();
    m_dialogState.reset(new CDialogState());
    //m_dialogState = std::move(dialog_state);
}

/*
CAbstractDialogStateManager::~CAbstractDialogStateManager()
{
    //safe_delete(m_dialogState)
    DEBUG_WITH_LINE << "QScopedPointer destructor";
}
*/

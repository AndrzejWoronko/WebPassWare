#include "AbstractCloseDialog.h"


namespace wpw {

CAbstractCloseDialog::CAbstractCloseDialog(const QString &dialogName, QWidget *parent)
    : CAbstractFieldsDialog(dialogName, parent), CAbstractDialogStateManager(), m_buttonBoxClose(nullptr)
{
}

CAbstractCloseDialog::~CAbstractCloseDialog()
{
    saveDialogState();
    DEBUG_WITH_LINE << "~dtor ";
}

void CAbstractCloseDialog::createButtons()
{
    m_buttonBoxClose = new CButtonBoxClose(this);
    connect(m_buttonBoxClose, &CButtonBoxClose::rejected, this, &CAbstractCloseDialog::reject);
    dialogLayout()->addWidget(m_buttonBoxClose);
    // The last step of addFieldsAndComponents(): the layout is complete.
    restoreDialogState();
}

void CAbstractCloseDialog::restoreDialogState()
{
    QByteArray state = getDialogState()->getState(getDialogName());
    this->restoreGeometry(state);
}

void CAbstractCloseDialog::saveDialogState()
{
    QByteArray state = this->saveGeometry();
    QByteArray oldState = getDialogState()->getState(getDialogName());

    if(state != oldState)
        getDialogState()->saveState(getDialogName(), state);
}

} // namespace wpw

#include "AbstractSettingsItemController.h"

CAbstractSettingsItemController::CAbstractSettingsItemController(QWidget *parent) : QWidget(parent)
{
    m_view = NULL;
}

CAbstractSettingsItemController::~CAbstractSettingsItemController()
{
    safe_delete(m_view)
}

void CAbstractSettingsItemController::enableButtons(void)
{
    if (m_view)
    {
        m_view->getButtonBox()->getCancelButton()->setEnabled(true);
        m_view->getButtonBox()->getSaveButton()->setEnabled(true);
    }
}

void CAbstractSettingsItemController::disableButtons(void)
{
    if (m_view)
    {
        m_view->getButtonBox()->getCancelButton()->setEnabled(false);
        m_view->getButtonBox()->getSaveButton()->setEnabled(false);
    }
}

#include "AbstractSettingsItemController.h"

namespace wpw {

CAbstractSettingsItemController::CAbstractSettingsItemController(QWidget *parent) : QObject(parent)
{
    m_view = nullptr;
}

CAbstractSettingsItemController::~CAbstractSettingsItemController()
{
    safe_delete(m_view);
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

} // namespace wpw

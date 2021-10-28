#include "SettingsDatabaseViewController.h"
#include "Application.h"

SettingsDatabaseViewController::SettingsDatabaseViewController(QWidget *parent) : CAbstractSettingsItemController(parent)
{
    m_view = new SettingsDatabaseView(this);

    connect(m_view, SIGNAL(changesWereMade()), this, SLOT(enableButtons()));
    SettingsDatabaseView* v = dynamic_cast<SettingsDatabaseView*>(m_view);
    if (v)
    {
        getValuesFromSettings();

    }
    //Przyciski
    connect(m_view->getButtonBox()->getSaveButton(), SIGNAL(clicked()), this, SLOT(setValuesToSettings()));
    connect(m_view->getButtonBox()->getRestoreButton(), SIGNAL(clicked()), this, SLOT(getValuesFromSettings()));
    connect(m_view->getButtonBox()->getCancelButton(), SIGNAL(clicked()), this, SLOT(getValuesFromSettings()));
}

void SettingsDatabaseViewController::getValuesFromSettings(void)
{
    //CFormAbstractField *f = NULL;
    this->disableButtons();
}

void SettingsDatabaseViewController::setValuesToSettings(void)
{
    //CFormAbstractField *f = NULL;
    this->disableButtons();
}

void SettingsDatabaseViewController::checkChanges()
{

}

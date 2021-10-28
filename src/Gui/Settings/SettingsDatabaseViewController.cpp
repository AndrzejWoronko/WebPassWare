#include "SettingsDatabaseViewController.h"
#include "Application.h"
#include "MessageBox.h"

SettingsDatabaseViewController::SettingsDatabaseViewController(QWidget *parent) : CAbstractSettingsItemController(parent)
{
    m_view = new SettingsDatabaseView(this);

    connect(m_view, SIGNAL(changesWereMade()), this, SLOT(enableButtons()));
    SettingsDatabaseView* v = dynamic_cast<SettingsDatabaseView*>(m_view);
    if (v)
    {
        getValuesFromSettings();
        connect(v->getBasePasswd1(), SIGNAL(textChanged(const QString&)), this, SLOT(enableButtons()));
        connect(v->getBasePasswd2(), SIGNAL(textChanged(const QString&)), this, SLOT(enableButtons()));
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
    CFormAbstractField *f = NULL;
    QString passwd1, passwd2;

    f = m_view->getFields().value("m_base_passwd1");
    if (f)
        passwd1 = f->getValue().toString();
    f = m_view->getFields().value("m_base_passwd2");
    if (f)
        passwd2 = f->getValue().toString();

    if (passwd1 == passwd2)
       {
           //Zapisz hasło do bazy

           this->disableButtons();
       }
    else
        CMessageBox::OkDialogWarning(tr("Hasła do bazy danych nie są jednakowe !!!"), m_view);

}

void SettingsDatabaseViewController::checkChanges()
{

}

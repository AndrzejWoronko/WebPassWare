#include "SettingsDatabaseViewController.h"
#include "Application.h"
#include "MessageBox.h"
#include "Database.h"

SettingsDatabaseViewController::SettingsDatabaseViewController(QWidget *parent) : CAbstractSettingsItemController(parent)
{
    m_view = new SettingsDatabaseView(this);

    connect(m_view, &SettingsDatabaseView::changesWereMade, this, &SettingsDatabaseViewController::enableButtons);
    SettingsDatabaseView* v = dynamic_cast<SettingsDatabaseView*>(m_view);
    if (v)
    {
        getValuesFromSettings();
        connect(v->getBasePasswd1(), &CFormTextField::textChanged, this, &SettingsDatabaseViewController::enableButtons);
        connect(v->getBasePasswd2(), &CFormTextField::textChanged, this, &SettingsDatabaseViewController::enableButtons);
    }
    //Przyciski
    connect(m_view->getButtonBox()->getSaveButton(), &QPushButton::clicked, this, &SettingsDatabaseViewController::setValuesToSettings);
    connect(m_view->getButtonBox()->getRestoreButton(), &QPushButton::clicked, this, &SettingsDatabaseViewController::getValuesFromSettings);
    connect(m_view->getButtonBox()->getCancelButton(), &QPushButton::clicked, this, &SettingsDatabaseViewController::getValuesFromSettings);
}

void SettingsDatabaseViewController::getValuesFromSettings(void)
{
    CFormAbstractField *f = NULL;
    QString passwd = DB.getDatabaseHashPassword();

    f = m_view->getFields().value("m_base_passwd1");
    if (f)
        f->setValue(passwd);
    f = m_view->getFields().value("m_base_passwd2");
    if (f)
        f->setValue(passwd);
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
           DB.setDatabasePassword(passwd1);
           this->disableButtons();
           CMessageBox::OkDialogInformation(tr("Hasła do bazy danych zostało zmienione !!!"), m_view);
       }
    else
        CMessageBox::OkDialogWarning(tr("Hasła do bazy danych nie są jednakowe !!!"), m_view);
}

void SettingsDatabaseViewController::checkChanges()
{

}

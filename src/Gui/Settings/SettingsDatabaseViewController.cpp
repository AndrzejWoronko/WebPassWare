#include "SettingsDatabaseViewController.h"
#include "Application.h"
#include "MessageBox.h"
#include "Database.h"

SettingsDatabaseViewController::SettingsDatabaseViewController(CDatabase &db, QWidget *parent) :
    CAbstractSettingsItemController(parent), m_db(db)
{
    m_view = new SettingsDatabaseView(m_db);

    connect(m_view, &SettingsDatabaseView::changesWereMade, this, &SettingsDatabaseViewController::enableButtons);
    SettingsDatabaseView* v = dynamic_cast<SettingsDatabaseView*>(m_view);
    if (v)
    {
        getValuesFromSettings();
        connect(v->getBasePasswdCurrent(), &CFormTextField::textChanged, this, &SettingsDatabaseViewController::enableButtons);
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
    // The fields never show the stored hash: saving it unchanged would have
    // hashed the hash and locked the user out.
    for (const QString &name : {QString("m_base_passwd_current"), QString("m_base_passwd1"), QString("m_base_passwd2")})
    {
        CFormAbstractField *f = m_view->getFields().value(name);
        if (f)
        {
            f->setValue(QString());
        }
    }
    this->disableButtons();
}

void SettingsDatabaseViewController::setValuesToSettings(void)
{
    const auto fieldValue = [this](const QString &name) {
        CFormAbstractField *f = m_view->getFields().value(name);
        return f ? f->getValue().toString() : QString();
    };
    const QString current = fieldValue("m_base_passwd_current");
    const QString passwd1 = fieldValue("m_base_passwd1");
    const QString passwd2 = fieldValue("m_base_passwd2");

    if (m_db.hasDatabasePassword() && !m_db.checkDatabasePassword(current))
    {
        CMessageBox::OkDialogWarning(tr("Obecne hasło jest niepoprawne !!!"), m_view);
        return;
    }
    if (passwd1.isEmpty())
    {
        CMessageBox::OkDialogWarning(tr("Hasło nie może być puste !!!"), m_view);
        return;
    }
    if (passwd1 != passwd2)
    {
        CMessageBox::OkDialogWarning(tr("Hasła do bazy danych nie są jednakowe !!!"), m_view);
        return;
    }
    m_db.setDatabasePassword(passwd1);
    getValuesFromSettings();
    CMessageBox::OkDialogInformation(tr("Hasła do bazy danych zostało zmienione !!!"), m_view);
}

void SettingsDatabaseViewController::checkChanges()
{

}

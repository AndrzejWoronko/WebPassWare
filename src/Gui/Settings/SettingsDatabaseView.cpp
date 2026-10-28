#include "SettingsDatabaseView.h"
#include "Database.h"

namespace wpw {

SettingsDatabaseView::SettingsDatabaseView(CDatabase &db, QWidget *parent) :
    CAbstractSettingsItemView(tr("Ustawienia bazy danych"), parent), m_db(db)
{
    addFieldsAndComponents();
}

SettingsDatabaseView::~SettingsDatabaseView()
{

}

void SettingsDatabaseView::addFields()
{
    m_base_type = new CFormTextField(QString("m_base_type"), tr("Typ bazy danych"), m_db.getDb().driverName(), 100, true);
    addField(m_base_type);
    m_base_name = new CFormTextField(QString("m_base_name"), tr("Plik bazy danych"), m_db.getBaseName(), 100, true);
    addField(m_base_name);
    m_base_full_path = new CFormTextField(QString("m_base_full_path"), tr("Ścieżka do bazy danych"), m_db.getBaseNameFullPath(), 100, true);
    addField(m_base_full_path);
    m_base_passwd_current = new CFormTextField(QString("m_base_passwd_current"), tr("Obecne hasło"), "", 100);
    m_base_passwd_current->setPasswdEcho();
    addField(m_base_passwd_current);
    m_base_passwd1 = new CFormTextField(QString("m_base_passwd1"), tr("Nowe hasło bazy danych"), "", 100);
    m_base_passwd1->setPasswdEcho();
    addField(m_base_passwd1);
    m_base_passwd2 = new CFormTextField(QString("m_base_passwd2"), tr("Powtorz hasło bazy danych"), "", 100);
    m_base_passwd2->setPasswdEcho();
    addField(m_base_passwd2);
}

void SettingsDatabaseView::addComponents()
{
//Dodajemy od wirsza nr 1 gdyż 0 jest tytuł

    m_connection_group_box = new QGroupBox(tr("Połączenie z bazą danych"), this);
    auto connectionGridLayout = new CGridLayout(m_connection_group_box);
    CForm::setComponents(connectionGridLayout, m_base_type, m_base_type->getLabel(), 1, 1, 1, 1);
    CForm::setComponents(connectionGridLayout, m_base_name, m_base_name->getLabel(), 2, 1, 1, 1);
    CForm::setComponents(connectionGridLayout, m_base_full_path, m_base_full_path->getLabel(), 3, 1, 1, 2);
    CForm::setComponents(connectionGridLayout, m_base_passwd_current, m_base_passwd_current->getLabel(), 4, 1, 1, 1);
    CForm::setComponents(connectionGridLayout, m_base_passwd1, m_base_passwd1->getLabel(), 5, 1, 1, 1);
    CForm::setComponents(connectionGridLayout, m_base_passwd2, m_base_passwd2->getLabel(), 6, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_connection_group_box, QString(""), 0, 1, 6, 3);
    setWidgetWidget();
}

} // namespace wpw

#include "SettingsDatabaseView.h"
#include "Database.h"

SettingsDatabaseView::SettingsDatabaseView(QWidget *parent) :
    CAbstractSettingsItemView(tr("Ustawienia bazy danych"), parent)
{
    addFieldsAndComponents();
}

SettingsDatabaseView::~SettingsDatabaseView()
{

}

void SettingsDatabaseView::addFields()
{
    m_base_type = new CFormTextField(QString("m_base_type"), tr("Typ bazy danych"), DB.getDb().driverName(), 100, true);
    addField(m_base_type);
    m_base_name = new CFormTextField(QString("m_base_name"), tr("Plik bazy danych"), DB.getBaseName(), 100, true);
    addField(m_base_name);
    m_base_full_path = new CFormTextField(QString("m_base_full_path"), tr("Ścieżka do bazy danych"), DB.getBaseNameFullPath(), 100, true);
    addField(m_base_full_path);
    m_base_passwd1 = new CFormTextField(QString("m_base_passwd1"), tr("Hasło bazy danych"), "", 100);
    addField(m_base_passwd1);
    m_base_passwd2 = new CFormTextField(QString("m_base_passwd2"), tr("Powtorz hasło bazy danych"), "", 100);
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
    CForm::setComponents(connectionGridLayout, m_base_passwd1, m_base_passwd1->getLabel(), 4, 1, 1, 1);
    CForm::setComponents(connectionGridLayout, m_base_passwd2, m_base_passwd2->getLabel(), 5, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_connection_group_box, QString(""), 0, 1, 5, 3);
    setWidgetWidget();
}

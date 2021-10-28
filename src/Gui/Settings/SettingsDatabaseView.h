#ifndef SETTINGSDATABASEVIEW_H
#define SETTINGSDATABASEVIEW_H


#include "Global.h"
#include "AbstractSettingsItemView.h"
#include <QGroupBox>

class SettingsDatabaseView : public CAbstractSettingsItemView
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CFormTextField, m_base_type, getBaseType)
    ADD_PTR_PROPERTY(CFormTextField, m_base_name, getBaseName)
    ADD_PTR_PROPERTY(CFormTextField, m_base_full_path, getBaseFullPath)
    ADD_PTR_PROPERTY(CFormTextField, m_base_passwd1, getBasePasswd1)
    ADD_PTR_PROPERTY(CFormTextField, m_base_passwd2, getBasePasswd2)

    ADD_PTR_PROPERTY(QGroupBox, m_connection_group_box, getConnectionGroupBox)
public:

     SettingsDatabaseView(QWidget *parent = NULL);
    ~SettingsDatabaseView();

    //Funkcja twprząca pola i dodająca pola do listy
    virtual void addFields();

    //Funkcja umieszczająca pola na formatce
    virtual void addComponents();
};

#endif // SETTINGSDATABASEVIEW_H

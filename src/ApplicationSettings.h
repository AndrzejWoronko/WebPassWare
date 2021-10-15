#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "Global.h"
#include <QMutex>

#ifndef APP_NAME
#define APP_NAME "WebPassWare"
#endif

#ifndef APP_VERSION
#define APP_VERSION WEBPASSWARE_VERSION_STR
#endif

#ifndef APP_ORGANIZATION_NAME
#define APP_ORGANIZATION_NAME "AWSoftware"
#endif

#ifndef APP_ORGANIZATIO_DOMAIN
#define APP_ORGANIZATIO_DOMAIN "awsoftware.com.pl"
//#define APP_ORGANIZATIO_DOMAIN "awsoftware.pl"
#endif

#ifndef APP_ICON
#define APP_ICON "WebPassWare"
#endif

#ifndef APP_DIR
#define APP_DIR "/opt/WebPassWare/"
#endif

//SETTINGS
//--GUI - configuration
#define SETTINGS_GUI_PREFIX QString::fromUtf8("GUI/")
#define SETTINGS_GUI_SHOW_GRID SETTINGS_GUI_PREFIX + "ShowGrid"
#define SETTINGS_GUI_SHOW_GRID_ROW_COLORS SETTINGS_GUI_PREFIX + "ShowGridRowColors"
#define SETTINGS_GUI_ROW_HEIGHT SETTINGS_GUI_PREFIX + "RowHeight"

#define SETTINGS_GUI_STYLE SETTINGS_GUI_PREFIX + "Style"
#define SETTINGS_GUI_THEME SETTINGS_GUI_PREFIX + "Theme"
#define SETTINGS_GUI_FONT SETTINGS_GUI_PREFIX + "Font"
#define SETTINGS_GUI_FONT_SIZE SETTINGS_GUI_PREFIX + "FontSize"

#define SETTINGS_GUI_DIALOG_MIN_HEIGHT SETTINGS_GUI_PREFIX + "DialogMinHeight"
#define SETTINGS_GUI_DIALOG_MIN_WIDTH SETTINGS_GUI_PREFIX + "DialogMinWidth"

#define SETTINGS_GUI_LOGIN_DIALOG_MIN_HEIGHT SETTINGS_GUI_PREFIX + "LoginDialogMinHeight"
#define SETTINGS_GUI_LOGIN_DIALOG_MIN_WIDTH SETTINGS_GUI_PREFIX + "LoginDialogMinWidth"

#define SETTINGS_GUI_LAYOUT_SPACING SETTINGS_GUI_PREFIX + "LayoutSpacing"
#define SETTINGS_GUI_LAYOUT_MARGIN SETTINGS_GUI_PREFIX + "LayoutMargin"


#define SETT CApplicationSettings::getInstance()

class CApplicationSettings : public Singleton<CApplicationSettings>
{
    ADD_PTR(QSettings, m_settings, getSettings)

public:

    CApplicationSettings();
    ~CApplicationSettings();

    void setValue(const QString &name, const QVariant &value);
    void setValueIf(const QString &name, const QVariant &value);
    QVariant getValue(const QString &name, const QVariant &def_value = QVariant(0));
    void setDefaultGuiSettings(void);

private:

    QMutex m_settingsMutex;
};

#endif // APPLICATIONSETTINGS_H

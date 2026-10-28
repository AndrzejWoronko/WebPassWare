#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "Global.h"
#include <QSettings>

namespace wpw {

inline constexpr char APP_NAME[] = "WebPassWare";

inline constexpr char APP_VERSION[] = WEBPASSWARE_VERSION_STR;

inline constexpr char APP_ORGANIZATION_NAME[] = "AWSoftware";

inline constexpr char APP_ORGANIZATIO_DOMAIN[] = "awsoftware.com.pl";

inline constexpr char APP_ICON[] = "WebPassWare";

inline constexpr char APP_DIR[] = "/opt/WebPassWare/";

//SETTINGS
//--GUI - configuration
inline constexpr char SETTINGS_GUI_SHOW_GRID[] = "GUI/ShowGrid";
inline constexpr char SETTINGS_GUI_SHOW_GRID_ROW_COLORS[] = "GUI/ShowGridRowColors";
inline constexpr char SETTINGS_GUI_ROW_HEIGHT[] = "GUI/RowHeight";

inline constexpr char SETTINGS_GUI_STYLE[] = "GUI/Style";
inline constexpr char SETTINGS_GUI_THEME[] = "GUI/Theme";
inline constexpr char SETTINGS_GUI_FONT[] = "GUI/Font";
inline constexpr char SETTINGS_GUI_FONT_SIZE[] = "GUI/FontSize";

inline constexpr char SETTINGS_GUI_DIALOG_MIN_HEIGHT[] = "GUI/DialogMinHeight";
inline constexpr char SETTINGS_GUI_DIALOG_MIN_WIDTH[] = "GUI/DialogMinWidth";

inline constexpr char SETTINGS_GUI_LOGIN_DIALOG_MIN_HEIGHT[] = "GUI/LoginDialogMinHeight";
inline constexpr char SETTINGS_GUI_LOGIN_DIALOG_MIN_WIDTH[] = "GUI/LoginDialogMinWidth";

inline constexpr char SETTINGS_GUI_LAYOUT_SPACING[] = "GUI/LayoutSpacing";
inline constexpr char SETTINGS_GUI_LAYOUT_MARGIN[] = "GUI/LayoutMargin";


// The program's settings in its settings file (QSettings with the
// organisation and name CApplication sets at start). Create one where it is
// needed; there is no shared instance.
class CApplicationSettings
{
public:

    CApplicationSettings() = default;


    void setValue(const QString &name, const QVariant &value);
    void setValueIf(const QString &name, const QVariant &value);
    QVariant getValue(const QString &name, const QVariant &def_value = QVariant(0));
    void setDefaultGuiSettings(void);
    QString fileName() const;

private:

    QSettings m_settings;
};

} // namespace wpw

#endif // APPLICATIONSETTINGS_H

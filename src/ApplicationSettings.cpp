#include "ApplicationSettings.h"
#include "Application.h"
#include "Layouts.h"

CApplicationSettings::CApplicationSettings()
{
    APPI->setAppInformation();
    m_settings.reset(new QSettings());
}

// CApplicationSettings::~CApplicationSettings()
// {
//     //safe_delete(m_settings)
// }

void CApplicationSettings::setValue(const QString &name, const QVariant &value)
{
    QMutexLocker lock(&m_settingsMutex);
    m_settings->setValue(name, value);
}

void CApplicationSettings::setValueIf(const QString &name, const QVariant &value)
{
    QMutexLocker lock(&m_settingsMutex);
    if (!m_settings->contains(name))
        m_settings->setValue(name, value);
}

QVariant CApplicationSettings::getValue(const QString &name, const QVariant &def_value)
{
    return  m_settings->value(name, def_value);
}

void CApplicationSettings::setDefaultGuiSettings(void)
{
    QFont f = APPI->font();
    this->setValueIf(SETTINGS_GUI_SHOW_GRID, QVariant(true));
    this->setValueIf(SETTINGS_GUI_SHOW_GRID_ROW_COLORS, QVariant(true));
    this->setValueIf(SETTINGS_GUI_ROW_HEIGHT, QVariant(2));
    this->setValueIf(SETTINGS_GUI_STYLE, QVariant(0));
    this->setValueIf(SETTINGS_GUI_THEME, QVariant(0));
    this->setValueIf(SETTINGS_GUI_FONT, QVariant(f.toString()));
    this->setValueIf(SETTINGS_GUI_DIALOG_MIN_HEIGHT, QVariant(240));
    this->setValueIf(SETTINGS_GUI_DIALOG_MIN_WIDTH, QVariant(320));
    this->setValueIf(SETTINGS_GUI_LOGIN_DIALOG_MIN_HEIGHT, QVariant(320));
    this->setValueIf(SETTINGS_GUI_LOGIN_DIALOG_MIN_WIDTH, QVariant(500));
    this->setValueIf(SETTINGS_GUI_LAYOUT_SPACING, QVariant(DEFAULT_LAYOUT_SPACING));
    this->setValueIf(SETTINGS_GUI_LAYOUT_MARGIN, QVariant(DEFAULT_LAYOUT_MARGIN));
}

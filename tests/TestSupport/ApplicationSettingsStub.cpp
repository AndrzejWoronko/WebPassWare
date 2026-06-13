#include "ApplicationSettings.h"

CApplicationSettings::CApplicationSettings()
{
    m_settings.reset(new QSettings());
}

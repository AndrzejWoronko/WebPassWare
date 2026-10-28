#include "Layouts.h"
#include "Application.h"

namespace wpw {

CVBoxLayout::CVBoxLayout(QWidget *parent) : QVBoxLayout(parent)
{
    int spacing = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_SPACING, DEFAULT_LAYOUT_SPACING).toInt();
    int margin = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_MARGIN, DEFAULT_LAYOUT_MARGIN).toInt();

    this->setSpacing(spacing);
    this->setContentsMargins(margin, margin, margin, margin);
}

CHBoxLayout::CHBoxLayout(QWidget *parent) : QHBoxLayout(parent)
{
    int spacing = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_SPACING, DEFAULT_LAYOUT_SPACING).toInt();
    int margin = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_MARGIN, DEFAULT_LAYOUT_MARGIN).toInt();

    this->setSpacing(spacing);
    this->setContentsMargins(margin, margin, margin, margin);
}

CGridLayout::CGridLayout(QWidget *parent) : QGridLayout(parent)
{
    int spacing = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_SPACING, DEFAULT_LAYOUT_SPACING).toInt();
    int margin = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_MARGIN, DEFAULT_LAYOUT_MARGIN).toInt();

    this->setSpacing(spacing);
    this->setContentsMargins(margin, margin, margin, margin);
}

CFormLayout::CFormLayout(QWidget *parent) : QFormLayout(parent)
{
    int spacing = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_SPACING, DEFAULT_LAYOUT_SPACING).toInt();
    int margin = CApplicationSettings().getValue(SETTINGS_GUI_LAYOUT_MARGIN, DEFAULT_LAYOUT_MARGIN).toInt();

    this->setSpacing(spacing);
    this->setContentsMargins(margin, margin, margin, margin);
}

} // namespace wpw

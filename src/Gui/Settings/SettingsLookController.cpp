#include "SettingsLookController.h"
#include "Application.h"

SettingsLookController::SettingsLookController(QWidget *parent) : CAbstractSettingsItemController(parent)
{
    m_view = new SettingsLookView(this);

    connect(m_view, &CAbstractSettingsItemView::changesWereMade, this, &SettingsLookController::enableButtons);

    auto v = dynamic_cast<SettingsLookView*>(m_view);
    if (v)
    {
        v->getColorSchema()->setValueList(CApplication::availableColors());
        v->getStyleTheme()->setValueList(CStyle::availableStyles());

        getValuesFromSettings();
        connect(v->getFontEdit(), &CFontEdit::fontChanged, APPI, &CApplication::updateFont);
        connect(v->getColorSchema(), qOverload<int>(&CFormSimpleIndexChoiceField::currentIndexChanged), APPI,  &CApplication::updatePalette);
        connect(v->getStyleTheme(), qOverload<int>(&CFormSimpleIndexChoiceField::currentIndexChanged), APPI,  &CApplication::updateStyleSheet);
        connect(v->getFontEdit(), &CFontEdit::fontChanged, this, &SettingsLookController::enableButtons);
        connect(v->getColorSchema(), qOverload<int>(&CFormSimpleIndexChoiceField::currentIndexChanged), this, &SettingsLookController::enableButtons);
        connect(v->getStyleTheme(), qOverload<int>(&CFormSimpleIndexChoiceField::currentIndexChanged), this, &SettingsLookController::enableButtons);

        connect(v->getTableShowGrid(), &CFormToggleSwitchField::checked, this, &SettingsLookController::enableButtons);
        connect(v->getTableAlterRowColor(), &CFormToggleSwitchField::checked, this, &SettingsLookController::enableButtons);
        connect(v->getTableRowHeight(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
        connect(v->getDialogsMinHeight(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
        connect(v->getDialogsMinWidth(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
        connect(v->getLayoutMargins(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
        connect(v->getLayoutSpacing(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
        connect(v->getLoginDialogMinHeight(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
        connect(v->getLoginDialogMinWidth(), qOverload<double>(&CFormNumberField::valueChanged), this, &SettingsLookController::enableButtons);
    }

    //Przyciski
    connect(m_view->getButtonBox()->getSaveButton(), &QPushButton::clicked, this, &SettingsLookController::setValuesToSettings);
    connect(m_view->getButtonBox()->getRestoreButton(), &QPushButton::clicked, this, &SettingsLookController::getValuesFromSettings);
    connect(m_view->getButtonBox()->getCancelButton(), &QPushButton::clicked, this, &SettingsLookController::getValuesFromSettings);

}

void SettingsLookController::getValuesFromSettings(void)
{
    CFormAbstractField *f = m_view->getFields().value("m_table_row_height");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_ROW_HEIGHT));
    f = m_view->getFields().value("m_table_show_grid");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_SHOW_GRID));
    f = m_view->getFields().value("m_table_alter_row_color");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_SHOW_GRID_ROW_COLORS));
    f = m_view->getFields().value("m_dialogs_min_width");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_DIALOG_MIN_WIDTH));
    f = m_view->getFields().value("m_dialogs_min_height");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_DIALOG_MIN_HEIGHT));
    f = m_view->getFields().value("m_layout_spacing");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_LAYOUT_SPACING));
    f = m_view->getFields().value("m_layout_margins");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_LAYOUT_MARGIN));
    f = m_view->getFields().value("m_login_dialog_min_width");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_LOGIN_DIALOG_MIN_WIDTH));
    f = m_view->getFields().value("m_login_dialog_min_height");
    if (f)
        f->setValue(SETT.getValue(SETTINGS_GUI_LOGIN_DIALOG_MIN_HEIGHT));

    auto v = dynamic_cast<SettingsLookView*>(m_view);
    if (v)
    {
        v->getColorSchema()->setIndexValue(SETT.getValue(SETTINGS_GUI_THEME).toInt());

        v->getStyleTheme()->setIndexValue(SETT.getValue(SETTINGS_GUI_STYLE).toInt());

        QFont font;
        QString s = SETT.getValue(SETTINGS_GUI_FONT).toString();
        if (font.fromString(s))
            v->getFontEdit()->setFont(font);
    }
    this->disableButtons();
}

void SettingsLookController::setValuesToSettings(void)
{
    CFormAbstractField *f = m_view->getFields().value("m_table_row_height");
    if (f)
        SETT.setValue(SETTINGS_GUI_ROW_HEIGHT, f->getValue());
    f = m_view->getFields().value("m_table_show_grid");
    if (f)
        SETT.setValue(SETTINGS_GUI_SHOW_GRID, f->getValue());
    f = m_view->getFields().value("m_table_alter_row_color");
    if (f)
        SETT.setValue(SETTINGS_GUI_SHOW_GRID_ROW_COLORS, f->getValue());
    f = m_view->getFields().value("m_dialogs_min_width");
    if (f)
        SETT.setValue(SETTINGS_GUI_DIALOG_MIN_WIDTH, f->getValue());
    f = m_view->getFields().value("m_dialogs_min_height");
    if (f)
        SETT.setValue(SETTINGS_GUI_DIALOG_MIN_HEIGHT, f->getValue());
    f = m_view->getFields().value("m_layout_spacing");
    if (f)
        SETT.setValue(SETTINGS_GUI_LAYOUT_SPACING, f->getValue());
    f = m_view->getFields().value("m_layout_margins");
    if (f)
        SETT.setValue(SETTINGS_GUI_LAYOUT_MARGIN, f->getValue());
    f = m_view->getFields().value("m_login_dialog_min_width");
    if (f)
        SETT.setValue(SETTINGS_GUI_LOGIN_DIALOG_MIN_WIDTH, f->getValue());
    f = m_view->getFields().value("m_login_dialog_min_height");
    if (f)
        SETT.setValue(SETTINGS_GUI_LOGIN_DIALOG_MIN_HEIGHT, f->getValue());

    auto v = dynamic_cast<SettingsLookView*>(m_view);
    if (v)
    {
        SETT.setValue(SETTINGS_GUI_THEME, v->getColorSchema()->getIndexValue());

        SETT.setValue(SETTINGS_GUI_STYLE, v->getStyleTheme()->getIndexValue());

        SETT.setValue(SETTINGS_GUI_FONT, v->getFontEdit()->getFont().toString());
    }
    this->disableButtons();
}

void SettingsLookController::checkChanges()
{

}

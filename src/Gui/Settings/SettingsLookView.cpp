#include "SettingsLookView.h"

SettingsLookView::SettingsLookView(QWidget *parent) : CAbstractSettingsItemView(tr("Ustawienia wyglądu"), parent)
{
   m_font_edit = new CFontEdit(this);
   addFieldsAndComponents();
}

SettingsLookView::~SettingsLookView()
{
    safe_delete(m_font_edit)
}

void SettingsLookView::addFields()
{
    m_color_schema = new CFormSimpleIndexChoiceField(QString("m_color_schema"), tr("Schemat kolorów"), 0, QStringList()/*APPI->availableColors()*/);
    addField(m_color_schema);
    m_style_theme = new CFormSimpleIndexChoiceField(QString("m_style_theme"), tr("Wybór motywu stylu"), 0, QStringList()/*STYLE->availableThemes()*/);
    addField(m_style_theme);
    m_table_row_height = new CFormNumberField(QString("m_table_row_height"), tr("Wysokość wiersza w tabelach"), 0, 1);
    addField(m_table_row_height);
    m_dialogs_min_height = new CFormNumberField(QString("m_dialogs_min_height"), tr("Minimalna wysokość dialogów (w pikselach)"), 0);
    addField(m_dialogs_min_height);
    m_dialogs_min_width = new CFormNumberField(QString("m_dialogs_min_width"), tr("Minimalna szerokość dialogów (w pikselach)"), 0);
    addField(m_dialogs_min_width);
    m_table_alter_row_color = new CFormCheckField(QString("m_table_alter_row_color"), tr("Inny color w wierszach w tabelach"), true);
    addField(m_table_alter_row_color);
    m_table_show_grid = new CFormCheckField(QString("m_table_show_grid"), tr("Pokaż siatkę w tabelach"), true);
    addField(m_table_show_grid);
    m_layout_spacing = new CFormNumberField(QString("m_layout_spacing"), tr("Odstęp pomiędzy elementami graficznymi"), 0);
    addField(m_layout_spacing);
    m_layout_margins = new CFormNumberField(QString("m_layout_margins"), tr("Margines wokół elementów graficznych"), 0);
    addField(m_layout_margins);
}

void SettingsLookView::addComponents()
{
//Dodajemy od wirsza nr 1 gdyż 0 jest tytuł
    CForm::setComponents(getFormLayout(), m_font_edit, tr("Wybór czcionki:"), 1, 1, 1, 3);
    CForm::setComponents(getFormLayout(), m_color_schema, m_color_schema->getLabel(), 2, 1, 1, 2);
    CForm::setComponents(getFormLayout(), m_style_theme, m_style_theme->getLabel(), 3, 1, 1, 2);
    CForm::setComponents(getFormLayout(), m_dialogs_min_height, m_dialogs_min_height->getLabel(), 4, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_dialogs_min_width, m_dialogs_min_width->getLabel(), 5, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_table_row_height, m_table_row_height->getLabel(), 6, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_table_alter_row_color, m_table_alter_row_color->getLabel(), 7, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_table_show_grid, m_table_show_grid->getLabel(), 8, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_layout_spacing, m_layout_spacing->getLabel(), 9, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_layout_margins, m_layout_margins->getLabel(), 10, 1, 1, 1);
    setWidgetWidget();
}

#ifndef SETTINGSLOOKVIEW_H
#define SETTINGSLOOKVIEW_H

#include "Global.h"
#include "AbstractSettingsItemView.h"
#include "FontEdit.h"

class SettingsLookView : public CAbstractSettingsItemView
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CFormSimpleIndexChoiceField, m_color_schema, getColorSchema)
    ADD_PTR_PROPERTY(CFormSimpleIndexChoiceField, m_style_theme, getStyleTheme)
    ADD_PTR_PROPERTY(CFontEdit, m_font_edit, getFontEdit)
    ADD_PTR_PROPERTY(CFormNumberField, m_table_row_height, getTableRowHeight)
    ADD_PTR_PROPERTY(CFormNumberField, m_dialogs_min_height, getDialogsMinHeight)
    ADD_PTR_PROPERTY(CFormNumberField, m_dialogs_min_width, getDialogsMinWidth)
    ADD_PTR_PROPERTY(CFormCheckField, m_table_alter_row_color, getTableAlterRowColor)
    ADD_PTR_PROPERTY(CFormCheckField, m_table_show_grid, getTableShowGrid)
    ADD_PTR_PROPERTY(CFormNumberField, m_layout_spacing, getLayoutSpacing) //Odstęp pomiędzy widgetami
    ADD_PTR_PROPERTY(CFormNumberField, m_layout_margins, getLayoutMargins) //Margines wokół widgetów
    ADD_PTR_PROPERTY(CFormNumberField, m_login_dialog_min_height, getLoginDialogMinHeight)
    ADD_PTR_PROPERTY(CFormNumberField, m_login_dialog_min_width, getLoginDialogMinWidth)

public:
     SettingsLookView(QWidget *parent = NULL);
    ~SettingsLookView();

    //Funkcja twprząca pola i dodająca pola do listy
    virtual void addFields();

    //Funkcja umieszczająca pola na formatce
    virtual void addComponents();
};

#endif // SETTINGSLOOKVIEW_H

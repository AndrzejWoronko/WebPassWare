#ifndef CABSTRACTSETTINGSITEMVIEW_H
#define CABSTRACTSETTINGSITEMVIEW_H

#include "Global.h"
#include "AbstractFormWidget.h"
#include <QScrollArea>

class CAbstractSettingsItemView : public QWidget
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CLabel, m_title_label, getTitleLabel) //Label z tytułem
    ADD_PTR_PROPERTY(CButtonBoxApplyRestore, m_button_box, getButtonBox)
    ADD_PTR_PROPERTY(CGridLayout, m_formLayout, getFormLayout) //layout widget
    ADD_PTR_PROPERTY(QWidget, m_widget, getWidget) //Widget do pól
    ADD_PTR_PROPERTY(QScrollArea, m_scrollArea, getScrollArea) //Scrollarea

public:
     explicit CAbstractSettingsItemView(const QString &title = QString(), QWidget *parent = NULL);
    ~CAbstractSettingsItemView();

    void addFieldsAndComponents();

    FormFieldHashList getFields();

    void addField(CFormAbstractField *f);

    void setWidgetWidget(void);
    void setWidgetWidget(QTabWidget *tabWidget);
    void setWidgetWidget(QWidget *widget);

private:
    FormFieldHashList m_fields;

    CVBoxLayout *m_VLayoutWidget;  //Podział na 3 części widget`a

public:

    //Virtualne funkcje do dodawnia komponentów i pól
    virtual void addComponents() = 0;
    virtual void addFields() = 0;

signals:
    void changesWereMade();

public slots:
    void emitChangesWereMade();

};

#endif // CABSTRACTSETTINGSITEMVIEW_H

#ifndef CABSTRACTFIELDSDIALOG_H
#define CABSTRACTFIELDSDIALOG_H

#include "Global.h"
#include "Form.h"

// Base of the dialogs built from fields (Template Method): addFieldsAndComponents()
// calls addFields(), addComponents() and createButtons(); subclasses choose
// the buttons. The fields go into getWidget() / getFormLayout(), which
// setDialogWidget() puts into the dialog.
class CAbstractFieldsDialog : public CDialog
{
    Q_OBJECT

    ADD_FIELD(QString, m_dialog_name, getDialogName, setDialogName)
    ADD_PTR_PROPERTY(CGridLayout, m_formLayout, getFormLayout) //layout widget
    ADD_PTR_PROPERTY(QWidget, m_widget, getWidget) //Widget do pól

public:
    explicit CAbstractFieldsDialog(const QString &dialogName, QWidget *parent = nullptr);
    ~CAbstractFieldsDialog();

    void addFieldsAndComponents();

    void setDialogWidget(void);
    void setDialogWidget(QTabWidget *tabWidget);
    void setDialogWidget(QWidget *widget);

    //Virtualne funkcje do dodawnia komponentów i pól
    virtual void addComponents() = 0;
    virtual void addFields() = 0;

protected:
    // Adds the buttons below the fields.
    virtual void createButtons() = 0;
    CVBoxLayout *dialogLayout();

private:
    CVBoxLayout *m_VLayoutDialog;  //Do dialogu
};

#endif // CABSTRACTFIELDSDIALOG_H

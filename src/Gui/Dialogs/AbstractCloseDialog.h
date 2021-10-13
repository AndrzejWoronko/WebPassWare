#ifndef CABSTRACTCLOSEDIALOG_H
#define CABSTRACTCLOSEDIALOG_H

#include "Global.h"
#include "Form.h"
#include "AbstractDialogStateManager.h"

class CAbstractCloseDialog : public CDialog, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_FIELD(QString, m_dialog_name, getDialogName, setDialogName)

    ADD_PTR_PROPERTY(CButtonBoxClose, m_buttonBoxClose, getButtonBoxClose)
    ADD_PTR_PROPERTY(CGridLayout, m_formLayout, getFormLayout) //layout widget
    ADD_PTR_PROPERTY(QWidget, m_widget, getWidget) //Widget do pól

public:

    explicit CAbstractCloseDialog(const QString &dialogName, QWidget *parent = NULL);
    ~CAbstractCloseDialog();

    void addFieldsAndComponents();

//    void closeEvent(QCloseEvent *event);

    void setDialogWidget(void);
    void setDialogWidget(QTabWidget *tabWidget);
    void setDialogWidget(QWidget *widget);

    virtual void restoreDialogState();
    virtual void saveDialogState();

private:

    CVBoxLayout *m_VLayoutDialog;  //Do dialogu
    void createButtons();

public:

    //Virtualne funkcje do dodawnia komponentów i pól
    virtual void addComponents() = 0;
    virtual void addFields() = 0;
};

#endif // CABSTRACTCLOSEDIALOG_H

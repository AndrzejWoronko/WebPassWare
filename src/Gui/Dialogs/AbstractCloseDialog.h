#ifndef CABSTRACTCLOSEDIALOG_H
#define CABSTRACTCLOSEDIALOG_H

#include "Global.h"
#include "Form.h"
#include "AbstractDialogStateManager.h"

class ICCloseDialog
{
public:
    ICCloseDialog() = default;
    virtual ~ICCloseDialog() = default;

//Virtualne funkcje do dodawnia komponentów i pól
    virtual void addComponents() = 0;
    virtual void addFields() = 0;
};


class CAbstractCloseDialog : public CDialog, public ICCloseDialog, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_FIELD(QString, m_dialog_name, getDialogName, setDialogName)

    ADD_QSMART_SHARED_PTR_PROPERTY(CButtonBoxClose, m_buttonBoxClose, getButtonBoxClose)
    ADD_PTR(CGridLayout, m_formLayout, getFormLayout) //layout widget
    ADD_QSMART_SHARED_PTR_PROPERTY(QWidget, m_widget, getWidget) //Widget do pól

public:

    explicit CAbstractCloseDialog(const QString &dialogName, QWidget *parent = NULL);
    ~CAbstractCloseDialog();

    void addFieldsAndComponents();

//    void closeEvent(QCloseEvent *event);

    void setDialogWidget(void);
    void setDialogWidget(QTabWidget *tabWidget);
    void setDialogWidget(QWidget *widget);

    virtual void restoreDialogState() override final;
    virtual void saveDialogState() override final;

private:

    QSharedPointer<CVBoxLayout> m_VLayoutDialog;  //Do dialogu
    void createButtons();
};

#endif // CABSTRACTCLOSEDIALOG_H

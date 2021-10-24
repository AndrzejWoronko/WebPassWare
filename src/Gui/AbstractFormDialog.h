#ifndef CABSTRACTFORMDIALOG_H
#define CABSTRACTFORMDIALOG_H

#include "Global.h"
#include "Form.h"

class CAbstractFormDialog : public CDialog
{
    Q_OBJECT

    ADD_FIELD(QString, m_dialog_name, getDialogName, setDialogName)
    /**
      ButtonBox zawierające przyciski potwierdzenia bądź zaprzestania dokonywanej operacji
    */
    ADD_PTR_PROPERTY(CButtonBox, m_buttonBox, getButtonBox)
    ADD_PTR_PROPERTY(CGridLayout, m_formLayout, getFormLayout) //layout widget
    ADD_PTR_PROPERTY(QWidget, m_widget, getWidget) //Widget do pól

    public:

    explicit CAbstractFormDialog(const QString &dialog_name, QWidget *parent = NULL);
    ~CAbstractFormDialog();

    void addFieldsAndComponents();

    void closeEvent(QCloseEvent *event);

    FormFieldHashList &getFields();

    void addField(CFormAbstractField *f);

    void setDialogWidget(void);
    void setDialogWidget(QTabWidget *tabWidget);
    void setDialogWidget(QWidget *widget);

private:
    //FormFieldList m_fields;
    FormFieldHashList m_fields;

    CVBoxLayout *m_VLayoutDialog;  //Do dialogu

    void createButtons();

public:

    //Virtualne funkcje do dodawnia komponentów i pól
    virtual void addComponents() = 0;
    virtual void addFields() = 0;
};

#endif // CABSTRACTFORMDIALOG_H

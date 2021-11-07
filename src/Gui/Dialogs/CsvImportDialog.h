#ifndef CCSVIMPORTDIALOG_H
#define CCSVIMPORTDIALOG_H

#include "Global.h"
#include "Dialog.h"
#include "Button.h"
#include "AbstractFormDialog.h"
#include "TableView.h"
#include <QStackedWidget>

class CCsvImportDialog : public CDialog
{
    Q_OBJECT
//First page
    ADD_PTR_PROPERTY(CFormTextField, m_fileNameCsv, getFileNameCsv)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_digitSign, getDigitSign)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_fileCodec, getFileCodec)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_fieldsSeparator, getFieldsSeparator)
    ADD_PTR_PROPERTY(CFormCheckField,  m_isTitle, getIsTitle)
    ADD_PTR_PROPERTY(CFormNumberField,  m_fromLine, getFromLine)
    ADD_PTR_PROPERTY(CTableView, m_tableView, getTableView)
//Second page
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_titleChoice, getTitleChoice)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_userChoice, getUserChoice)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_passChoice, getPassChoice)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_webChoice, getWebChoice)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_descChoice, getDescChoice)
    ADD_PTR_PROPERTY(CTableView, m_tableViewPreview, getTableViewPreview)

//Przyciski
    ADD_PTR_PROPERTY(CButton, m_buttonChioceFile, getButtonChoiceFile)
    ADD_PTR_PROPERTY(CButton, m_buttonOk, getButtonOk)
    ADD_PTR_PROPERTY(CButton, m_buttonCancel, getButtonCancel)
    ADD_PTR_PROPERTY(CButton, m_buttonBack, getButtonBack)
    ADD_PTR_PROPERTY(CButton, m_buttonNext, getButtonNext)

    ADD_PTR_PROPERTY(QStackedWidget, m_stackedWidget, getStackedWidget)

public:

    CCsvImportDialog(QWidget *parent = NULL);

private:
    void addFields();
    void addField(CFormAbstractField *f);
    void addComponents();
    void addButtons();

    QStringList getDigitSigns(void);
    QStringList getFileCodecs(void);
    QStringList getFieldsSeparators(void);

    FormFieldHashList m_fields;

    CHBoxLayout *m_buttonLayout; //layout do przycisków

    CGridLayout *m_formLayout_first; //layout do m_widget
    QWidget *m_widget_first; //Widget do pól

    CGridLayout *m_formLayout_second; //layout do m_widget
    QWidget *m_widget_second; //Widget do pól

    CVBoxLayout *m_VLayoutDialog;  //Do dialogu
};

#endif // CCSVIMPORTDIALOG_H

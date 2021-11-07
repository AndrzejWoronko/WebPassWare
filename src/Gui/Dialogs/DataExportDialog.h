#ifndef CDATAEXPORTDIALOG_H
#define CDATAEXPORTDIALOG_H

#include "Global.h"
#include "Dialog.h"
#include "Button.h"
#include "ButtonBox.h"
#include "AbstractFormDialog.h"

class CDataExportDialog : public CDialog
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CFormTextField,   m_fileNameCsv, getFileNameCsv)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_digitSign, getDigitSign)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_fileCodec, getFileCodec)
    ADD_PTR_PROPERTY(CFormSimpleChoiceField, m_fieldsSeparator, getFieldsSeparator)

    ADD_PTR_PROPERTY(CButton, m_buttonChioceFileCsv, getButtonChoiceFileCsv)
    /**
      ButtonBox zawierające przyciski potwierdzenia bądź zaprzestania dokonywanej operacji
    */
    ADD_PTR_PROPERTY(CButtonBoxExport, m_buttonBox, getButtonBox)

public:

    CDataExportDialog(QWidget *parent = NULL);
    ~CDataExportDialog() {};

private:

    void addFields();
    void addField(CFormAbstractField *f);
    void addComponents();
    void addButtons();

    QStringList getDigitSigns(void);
    QStringList getFileCodecs(void);
    QStringList getFieldsSeparators(void);


    FormFieldHashList m_fields;

    CGridLayout *m_formLayout_file_csv; //layout do m_widget
    CHBoxLayout *m_buttonLayout; //layout do przycisków

    CVBoxLayout *m_VLayoutDialog;  //Do dialogu
};

#endif // CDATAEXPORTDIALOG_H

#ifndef CPASSWORDGENERATEDIALOG_H
#define CPASSWORDGENERATEDIALOG_H

#include "Global.h"
#include "AbstractCloseDialog.h"
#include "PasswordGeneratorWidget.h"

class CPasswordGenerateDialog : public CAbstractCloseDialog
{
    ADD_PTR_PROPERTY(CPasswordGeneratorWidget, m_pass_gen_widget, getPassGenWidget)

public:

    CPasswordGenerateDialog(QWidget *parent = NULL);
    ~CPasswordGenerateDialog();

    //Funkcja tworząca pola
    virtual void addFields() override final;

    //Funkcja umieszczająca pola na formatce
    virtual void addComponents() override final;
};

#endif // CPASSWORDGENERATEDIALOG_H

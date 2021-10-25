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
    virtual void addFields();

    //Funkcja umieszczająca pola na formatce
    virtual void addComponents();
};

#endif // CPASSWORDGENERATEDIALOG_H

#ifndef PASSENTRYDIALOGCONTROLLER_H
#define PASSENTRYDIALOGCONTROLLER_H

#include "Global.h"
#include "AbstractFormDialogController.h"
#include "PassEntryDialog.h"

class PassEntryDialogController : public CAbstractFormDialogController
{
    Q_OBJECT

public:

    explicit PassEntryDialogController(QWidget *parent = NULL);
    ~PassEntryDialogController();

    static QString validateForm(FormFieldHashList fields);
    static QString isEmptyForm(FormFieldHashList fields);
};

#endif // PASSENTRYDIALOGCONTROLLER_H

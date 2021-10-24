#ifndef PASSGROUPDIALOGCONTROLLER_H
#define PASSGROUPDIALOGCONTROLLER_H

#include "Global.h"
#include "AbstractFormDialogController.h"
#include "PassGroupDialog.h"

class PassGroupDialogController : public CAbstractFormDialogController
{
    Q_OBJECT

public:

    explicit PassGroupDialogController(QWidget *parent = NULL);
    ~PassGroupDialogController();

    static QString validateForm(FormFieldHashList fields);
    static QString isEmptyForm(FormFieldHashList fields);
};

#endif // PASSGROUPDIALOGCONTROLLER_H

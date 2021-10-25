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

    virtual bool exec(const QString &title);
    virtual bool exec(qint64 id, const QString &title);

private Q_SLOTS:
    void setGenPassword(const QString& password);
};

#endif // PASSENTRYDIALOGCONTROLLER_H

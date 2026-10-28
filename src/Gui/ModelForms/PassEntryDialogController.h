#ifndef PASSENTRYDIALOGCONTROLLER_H
#define PASSENTRYDIALOGCONTROLLER_H

#include "Global.h"
#include "AbstractFormDialogController.h"
#include "PassEntryDialog.h"

namespace wpw {

class PassEntryService;
class PassGroupService;

class PassEntryDialogController : public CAbstractFormDialogController
{
    Q_OBJECT

public:

    PassEntryDialogController(PassEntryService *passEntryService, PassGroupService *passGroupService, QWidget *parent = nullptr);
    ~PassEntryDialogController();

    static QString requiredFieldsError(FormFieldHashList fields);

    virtual bool exec(const QString &title);
    virtual bool exec(qint64 id, const QString &title);

private Q_SLOTS:
    void setGenPassword(const QString& password);

private:
    PassEntryService *m_passEntryService;
    PassGroupService *m_passGroupService;
};

} // namespace wpw

#endif // PASSENTRYDIALOGCONTROLLER_H

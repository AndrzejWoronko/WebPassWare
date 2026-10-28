#ifndef PASSGROUPDIALOGCONTROLLER_H
#define PASSGROUPDIALOGCONTROLLER_H

#include "Global.h"
#include "AbstractFormDialogController.h"
#include "PassGroupDialog.h"

namespace wpw {

class PassGroupService;

class PassGroupDialogController : public CAbstractFormDialogController
{
    Q_OBJECT

public:

    PassGroupDialogController(PassGroupService *passGroupService, QWidget *parent = nullptr);
    ~PassGroupDialogController();

    static QString requiredFieldsError(FormFieldHashList fields);

    virtual bool exec(const QString &title);
    virtual bool exec(qint64 id, const QString &title);

private:
    PassGroupService *m_passGroupService;
};

} // namespace wpw

#endif // PASSGROUPDIALOGCONTROLLER_H

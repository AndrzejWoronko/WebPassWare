#ifndef PASSGROUPDIALOGCONTROLLER_H
#define PASSGROUPDIALOGCONTROLLER_H

#include "Global.h"
#include "AbstractFormDialogController.h"
#include "PassGroupDialog.h"

class PassGroupService;

class PassGroupDialogController : public CAbstractFormDialogController
{
    Q_OBJECT

public:

    explicit PassGroupDialogController(QWidget *parent = nullptr);
    PassGroupDialogController(PassGroupService *passGroupService, QWidget *parent = nullptr);
    ~PassGroupDialogController();

    static QString validateForm(FormFieldHashList fields);
    static QString isEmptyForm(FormFieldHashList fields);

    virtual bool exec(const QString &title);
    virtual bool exec(qint64 id, const QString &title);

private:
    PassGroupService *m_passGroupService;
};

#endif // PASSGROUPDIALOGCONTROLLER_H

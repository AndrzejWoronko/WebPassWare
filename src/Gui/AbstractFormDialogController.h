#ifndef CABSTRACTFORMDIALOGCONTROLLER_H
#define CABSTRACTFORMDIALOGCONTROLLER_H

#include "Global.h"
#include <QWidget>
#include "AbstractDialogStateManager.h"
#include "AbstractFormDialog.h"
#include "FormValidator.h"
#include <QJsonObject>

class CAbstractFormDialogController : public QWidget, public CAbstractDialogStateManager
{
   Q_OBJECT

public:
    CAbstractFormDialogController(CAbstractFormDialog *dialog, QWidget *parent = NULL);
    ~CAbstractFormDialogController() {};

    virtual void restoreDialogState();
    virtual void saveDialogState();
    virtual void setConnections();

    CAbstractFormDialog *getDialog();

    bool isChangesOccured();

    QJsonObject toJson();

protected:

    CAbstractFormDialog *m_dialog;
    CFormValidator *m_validator;

    CForm::FormMode m_mode;

public slots:
    void checkChanges();
    void checkRequiredFields();

};

#endif // CABSTRACTFORMDIALOGCONTROLLER_H

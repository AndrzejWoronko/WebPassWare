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

    virtual bool exec(const QString &title) = 0;
    virtual bool exec(qint64 id, const QString &title) = 0;

    CAbstractFormDialog *getDialog();

    bool isChangesOccured();

    QJsonObject toJson();

protected:

    CAbstractFormDialog *m_dialog;
    CFormValidator *m_validator;

    CForm::FormMode m_mode;

    template<class FieldType>
    FieldType *getFieldAs(const QString &fieldName) const
    {
        if (!m_dialog)
        {
            return nullptr;
        }
        return dynamic_cast<FieldType*>(m_dialog->getFields().value(fieldName));
    }

    template<class DialogType>
    DialogType *getDialogAs() const
    {
        return dynamic_cast<DialogType*>(m_dialog);
    }

public slots:
    void checkChanges();
    void checkRequiredFields();

};

#endif // CABSTRACTFORMDIALOGCONTROLLER_H

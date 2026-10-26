#ifndef CABSTRACTCLOSEDIALOG_H
#define CABSTRACTCLOSEDIALOG_H

#include "Global.h"
#include "AbstractFieldsDialog.h"
#include "AbstractDialogStateManager.h"

// A dialog that only shows information, closed with one button; its geometry
// is saved and restored under getDialogName().
class CAbstractCloseDialog : public CAbstractFieldsDialog, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CButtonBoxClose, m_buttonBoxClose, getButtonBoxClose)

public:

    explicit CAbstractCloseDialog(const QString &dialogName, QWidget *parent = nullptr);
    ~CAbstractCloseDialog();

    virtual void restoreDialogState() override final;
    virtual void saveDialogState() override final;

protected:
    void createButtons() override;
};

#endif // CABSTRACTCLOSEDIALOG_H

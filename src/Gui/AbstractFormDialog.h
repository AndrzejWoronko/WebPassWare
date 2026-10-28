#ifndef CABSTRACTFORMDIALOG_H
#define CABSTRACTFORMDIALOG_H

#include "Global.h"
#include "AbstractFieldsDialog.h"

namespace wpw {

// A dialog with editable fields and OK / Cancel; the fields are registered with
// addField() so the controller can read and validate them.
class CAbstractFormDialog : public CAbstractFieldsDialog
{
    Q_OBJECT

    /**
      ButtonBox zawierające przyciski potwierdzenia bądź zaprzestania dokonywanej operacji
    */
    ADD_PTR_PROPERTY(CButtonBox, m_buttonBox, getButtonBox)

    public:

    explicit CAbstractFormDialog(const QString &dialog_name, QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event);

    FormFieldHashList &getFields();

    void addField(CFormAbstractField *f);

protected:
    void createButtons() override;

private:
    FormFieldHashList m_fields;
};

} // namespace wpw

#endif // CABSTRACTFORMDIALOG_H

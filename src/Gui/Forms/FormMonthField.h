#ifndef FORMMONTHFIELD_H
#define FORMMONTHFIELD_H

#include "Global.h"
#include <QDateEdit>
#include "DateEdit.h"
#include "FormAbstractField.h"

namespace wpw {

class CFormMonthField : public CMonthEdit, public CFormAbstractField
{

  public:
    CFormMonthField(const QString &variableName, const QString &label, const QDate &value, bool readOnly = false, QWidget *parent = nullptr);
    ~CFormMonthField();

    QVariant getValue();
    virtual void setValue(QVariant value);

    void setReadOnly();
    void setReadWrite();
    virtual void select();
};

} // namespace wpw

#endif // FORMMONTHFIELD_H

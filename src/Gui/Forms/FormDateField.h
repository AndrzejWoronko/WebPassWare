#ifndef FORMDATEFIELD_H
#define FORMDATEFIELD_H

#include "Global.h"
#include <QDateEdit>
#include "DateEdit.h"
#include "FormAbstractField.h"


namespace wpw {

class CFormDateField : public CDateEdit, public CFormAbstractField
{

  public:
    CFormDateField(const QString &variableName, const QString &label, const QDate &value, bool readOnly = false, QWidget *parent = nullptr);
    ~CFormDateField();

    QVariant getValue();    
    virtual void setValue(QVariant value);

    virtual void select();

    void setReadOnly();
    void setReadWrite();
};

} // namespace wpw

#endif // FORMDATEFIELD_H

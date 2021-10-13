#ifndef FORMMONTHFIELD_H
#define FORMMONTHFIELD_H

#include "Global.h"
#include <QDateEdit>
#include "DateEdit.h"
#include "FormAbstractField.h"

class CFormMonthField : public CMonthEdit, public CFormAbstractField
{

  public:
    CFormMonthField(const QString &variableName, const QString &label, const QDate &value, bool readOnly = false, QWidget *parent = NULL);
    ~CFormMonthField();

    QVariant getValue();
    virtual void setValue(QVariant value);

    void setReadOnly();
    void setReadWrite();
    virtual void select();
};


#endif // FORMMONTHFIELD_H

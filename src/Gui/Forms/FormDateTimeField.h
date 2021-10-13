#ifndef FORMDATETIMEFIELD_H
#define FORMDATETIMEFIELD_H

#include "Global.h"
#include <QDateTimeEdit>
#include "DateEdit.h"
#include "FormAbstractField.h"

class CFormDateTimeField : public CDateTimeEdit, public CFormAbstractField
{

  public:

    CFormDateTimeField(const QString &variableName, const QString &label, const QDateTime &value, bool readOnly = false, QWidget *parent = NULL);
    ~CFormDateTimeField();

    QVariant getValue();
    virtual void setValue(QVariant value);

    void setReadOnly();
    void setReadWrite();

    virtual void select();

};

#endif // FORMDATETIMEFIELD_H

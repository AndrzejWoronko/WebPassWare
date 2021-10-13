#ifndef CHECKFIELD_H
#define CHECKFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include <QCheckBox>

class CFormCheckField : public QCheckBox, public CFormAbstractField
{

  public:
    CFormCheckField(const QString &variableName, const QString &label, bool value, bool readOnly = false, QWidget *parent = NULL);
    ~CFormCheckField();

    QVariant getValue();    
    virtual void setValue(QVariant value);   

    void setReadOnly();
    void setReadWrite();
};

#endif // CHECKFIELD_H

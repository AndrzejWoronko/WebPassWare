#ifndef CFORMRADIOBUTTONFIELD_H
#define CFORMRADIOBUTTONFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include <QRadioButton>

class CFormRadioButtonField : public QRadioButton, public CFormAbstractField
{

  public:
    CFormRadioButtonField(const QString &variableName, const QString &label, bool value, QWidget *parent = NULL);
    ~CFormRadioButtonField();

    QVariant getValue();    
    virtual void setValue(QVariant value);
};


#endif // CFORMRADIOBUTTONFIELD_H

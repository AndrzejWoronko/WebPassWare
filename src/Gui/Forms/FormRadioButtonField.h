#ifndef CFORMRADIOBUTTONFIELD_H
#define CFORMRADIOBUTTONFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include <QRadioButton>

namespace wpw {

class CFormRadioButtonField : public QRadioButton, public CFormAbstractField
{

  public:
    CFormRadioButtonField(const QString &variableName, const QString &label, bool value, QWidget *parent = nullptr);
    ~CFormRadioButtonField();

    QVariant getValue();    
    virtual void setValue(QVariant value);
};

} // namespace wpw

#endif // CFORMRADIOBUTTONFIELD_H

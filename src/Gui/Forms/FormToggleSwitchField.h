#ifndef CFORMTOGGLESWITCHFIELD_H
#define CFORMTOGGLESWITCHFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include "ToggleSwitch.h"

class CFormToggleSwitchField : public CToggleSwitch, public CFormAbstractField
{

public:
    CFormToggleSwitchField(const QString &variableName, const QString &label, bool value, bool readOnly = false, QWidget *parent = NULL);
    ~CFormToggleSwitchField() = default;

    QVariant getValue();
    virtual void setValue(QVariant value);

    void setReadOnly();
    void setReadWrite();
};



#endif // CFORMTOGGLESWITCHFIELD_H

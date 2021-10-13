#include "FormRadioButtonField.h"

CFormRadioButtonField::CFormRadioButtonField(const QString &variableName, const QString &label, bool value, QWidget *parent) :
QRadioButton(parent), CFormAbstractField(QVariant::Bool, variableName, label, value)
{
    m_widget = this;
    this->setChecked(value);
}

CFormRadioButtonField::~CFormRadioButtonField()
{

}

QVariant CFormRadioButtonField::getValue()
{
    return this->isChecked();
}

void CFormRadioButtonField::setValue(QVariant value)
{
    setRawValue(value);
    this->setChecked(value.toBool());
}



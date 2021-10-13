#include "FormCheckField.h"


CFormCheckField::CFormCheckField(const QString &variableName, const QString &label, bool value, bool readOnly, QWidget *parent) :
QCheckBox(parent), CFormAbstractField(QVariant::Bool, variableName, label, value)
{
    m_widget = this;
    this->setChecked(value);

    if (readOnly)
        this->setReadOnly();
//    else
//        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

CFormCheckField::~CFormCheckField()
{

}

QVariant CFormCheckField::getValue()
{
    return this->isChecked();
}

void CFormCheckField::setValue(QVariant value)
{
    setRawValue(value);
    this->setChecked(value.toBool());
}

void CFormCheckField::setReadOnly()
{
    QCheckBox::setCheckable(false);
    QCheckBox::setFocusPolicy(Qt::NoFocus);
}

void CFormCheckField::setReadWrite()
{
    QCheckBox::setCheckable(true);
    QCheckBox::setFocusPolicy(Qt::StrongFocus);
}

#include "FormToggleSwitchField.h"

CFormToggleSwitchField::CFormToggleSwitchField(const QString &variableName, const QString &label, bool value, bool readOnly, QWidget *parent) :
    CToggleSwitch(parent), CFormAbstractField(QVariant::Bool, variableName, label, value)
{
    m_widget = this;
    this->setChecked(value);

    if (readOnly)
        this->setReadOnly();
    //    else
    //        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}


QVariant CFormToggleSwitchField::getValue()
{
    return this->isChecked();
}

void CFormToggleSwitchField::setValue(QVariant value)
{
    setRawValue(value);
    this->setChecked(value.toBool());
}

void CFormToggleSwitchField::setReadOnly()
{
    CToggleSwitch::setCheckable(false);
    QWidget::setFocusPolicy(Qt::NoFocus);
}

void CFormToggleSwitchField::setReadWrite()
{
    CToggleSwitch::setCheckable(true);
    QWidget::setFocusPolicy(Qt::StrongFocus);
}

#include "FormAbstractField.h"


CFormAbstractField::CFormAbstractField(QVariant::Type type, const QString &variableName, const QString &label, QVariant value):
                                       m_type(type),
                                       m_variableName(variableName),
                                       m_label(label),
                                       m_value(value)
{
    if (!m_label.isEmpty() && m_label.at(m_label.length() - 1) != ':')
        m_label.append(":");    
    m_widget = NULL;
    m_startValue = value;
}

CFormAbstractField::~CFormAbstractField()
{
}

QString CFormAbstractField::getVariableName()
{
    return m_variableName;
}

QString CFormAbstractField::getLabel()
{
    return m_label;
}

bool CFormAbstractField::isDirty()
{
    return m_startValue != getValue();
}

void CFormAbstractField::setStartValue(QVariant value)
{
    m_startValue = value;
}

QVariant CFormAbstractField::getStartValue(void)
{
    return m_startValue;
}

QVariant CFormAbstractField::getRawValue(void)
{
    return m_value;
}

void CFormAbstractField::setRawValue(QVariant value)
{
    m_value = value;
}

void CFormAbstractField::select()
{

}

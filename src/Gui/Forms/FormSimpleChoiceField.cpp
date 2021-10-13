#include "FormSimpleChoiceField.h"

CFormSimpleChoiceField::CFormSimpleChoiceField(const QString &variableName, const QString &label, QVariant value, const QStringList &valueList, QWidget *parent) :
QComboBox(parent), CFormAbstractField(QVariant::String, variableName, label, value)
{
   m_widget = this;
   if (!valueList.isEmpty())
       this->addItems(valueList);
   this->setMaxVisibleItems(15);
}

bool CFormSimpleChoiceField::isDirty()
{
    return m_startValue.toInt() != -1 && CFormAbstractField::isDirty();
}

QVariant CFormSimpleChoiceField::getValue()
{
    m_value = this->currentData(Qt::DisplayRole);
    return m_value;
}

void CFormSimpleChoiceField::setValue(QVariant value)
{
    setRawValue(value);
    this->setCurrentIndex(this->findData(m_value, Qt::DisplayRole, Qt::MatchExactly));
}

void CFormSimpleChoiceField::setValueList(const QStringList &valueList)
{
    this->clear();
    this->addItems(valueList);
}

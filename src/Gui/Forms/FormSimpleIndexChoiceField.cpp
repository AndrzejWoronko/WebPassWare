#include "FormSimpleIndexChoiceField.h"

CFormSimpleIndexChoiceField::CFormSimpleIndexChoiceField(const QString &variableName, const QString &label, QVariant value, const QStringList &valueList, QWidget *parent) :
CFormSimpleChoiceField(variableName, label, value, valueList, parent)
{

}

int CFormSimpleIndexChoiceField::getIndexValue()
{
    return this->currentIndex();
}

void CFormSimpleIndexChoiceField::setIndexValue(int idx)
{
    this->setCurrentIndex(idx);
}

#include "FormYesNoChoiceField.h"

CFormYesNoChoiceField::CFormYesNoChoiceField(const QString &variableName, const QString &label, bool value, QWidget *parent) :
CFormSimpleIndexChoiceField(variableName, label, value, QStringList(), parent)
{
   m_widget = this;
   m_valueList = QStringList() << tr("Nie") << tr("Tak");

   this->setValueList(m_valueList);
   this->setMaxVisibleItems(2);
}

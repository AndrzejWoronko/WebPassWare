#ifndef FORMYESNOCHOICEFIELD_H
#define FORMYESNOCHOICEFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include "FormSimpleIndexChoiceField.h"

class CFormYesNoChoiceField : public CFormSimpleIndexChoiceField
{

   QStringList  m_valueList;

public:
   CFormYesNoChoiceField(const QString &variableName, const QString &label, bool value, QWidget *parent = NULL);

};

#endif // FORMYESNOCHOICEFIELD_H

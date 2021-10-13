#ifndef FORMSIMPLEINDEXCHOICEFIELD_H
#define FORMSIMPLEINDEXCHOICEFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include "FormSimpleChoiceField.h"

class CFormSimpleIndexChoiceField : public CFormSimpleChoiceField
{

public:

    CFormSimpleIndexChoiceField(const QString &variableName, const QString &label, QVariant value, const QStringList &valueList = QStringList(), QWidget *parent = NULL);

    int getIndexValue(void);
    void setIndexValue(int);
};

#endif // FORMSIMPLEINDEXCHOICEFIELD_H

#ifndef FORMSIMPLECHOICEFIELD_H
#define FORMSIMPLECHOICEFIELD_H

#include "Global.h"
#include <QComboBox>
#include "FormAbstractField.h"

class CFormSimpleChoiceField : public QComboBox, public CFormAbstractField
{

public:

    CFormSimpleChoiceField(const QString &variableName, const QString &label, QVariant value, const QStringList &valueList = QStringList(), QWidget *parent = NULL);

    virtual QVariant getValue();    
    virtual void setValue(QVariant value);

    virtual bool isDirty();

    void setValueList(const QStringList &valueList);

};

#endif // FORMSIMPLECHOICEFIELD_H

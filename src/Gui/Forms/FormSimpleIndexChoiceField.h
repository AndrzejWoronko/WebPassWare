#ifndef FORMSIMPLEINDEXCHOICEFIELD_H
#define FORMSIMPLEINDEXCHOICEFIELD_H

#include "Global.h"
#include "FormAbstractField.h"
#include "FormSimpleChoiceField.h"

class CFormSimpleIndexChoiceField : public CFormSimpleChoiceField
{

public:

    CFormSimpleIndexChoiceField(const QString &variableName, const QString &label, QVariant value, const QStringList &valueList = QStringList(), QWidget *parent = nullptr);

    int getIndexValue(void);
    void setIndexValue(int);

    // Items that each carry an id (a database key) as item data, so the
    // selection does not depend on the item's position.
    void setIdChoices(const QList<QPair<qint64, QString>> &choices);
    // -1 without a selection.
    qint64 getIdValue(void);
    void setIdValue(qint64 id);
};

#endif // FORMSIMPLEINDEXCHOICEFIELD_H

#include "FormSimpleIndexChoiceField.h"

namespace wpw {

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

void CFormSimpleIndexChoiceField::setIdChoices(const QList<QPair<qint64, QString>> &choices)
{
    this->clear();
    for (const auto &choice : choices)
    {
        this->addItem(choice.second, QVariant(choice.first));
    }
}

qint64 CFormSimpleIndexChoiceField::getIdValue()
{
    const QVariant id = this->currentData();
    return id.isValid() ? id.toLongLong() : -1;
}

void CFormSimpleIndexChoiceField::setIdValue(qint64 id)
{
    this->setCurrentIndex(this->findData(QVariant(id)));
}

} // namespace wpw

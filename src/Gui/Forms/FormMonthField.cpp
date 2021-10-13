#include "FormMonthField.h"

CFormMonthField::CFormMonthField(const QString &variableName, const QString &label, const QDate &value, bool readOnly, QWidget *parent):
        CMonthEdit(value, parent), CFormAbstractField(QVariant::Date, variableName, label, value)
{
    m_widget = this;
    if (readOnly)
        this->setReadOnly();
    else
        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

CFormMonthField::~CFormMonthField()
{

}

QVariant CFormMonthField::getValue()
{
    QDate d;
    d.setDate(this->date().year(), this->date().month(), 1);
    return d;
}

void CFormMonthField::setValue(QVariant value)
{    
    if (value.type() == QVariant::Date)
        {        
            QDate d = value.toDate();
            d.setDate(d.year(), d.month(), 1);
            setRawValue(QVariant(d));
            this->setDate(d);
        }
}

void CFormMonthField::setReadOnly()
{
    CDateEdit::setReadOnly(true);
    CDateEdit::setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(QString());
}

void CFormMonthField::setReadWrite()
{
    CDateEdit::setReadOnly(false);
    CDateEdit::setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

void CFormMonthField::select()
{
    this->selectAll();
}

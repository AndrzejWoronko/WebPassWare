#include "FormDateTimeField.h"
#include "CalendarWidget.h"


CFormDateTimeField::CFormDateTimeField(const QString &variableName, const QString &label, const QDateTime &value, bool readOnly, QWidget *parent):
        CDateTimeEdit(value, parent), CFormAbstractField(QVariant::DateTime, variableName, label, value)
{
    m_widget = this;

    if (readOnly)
        this->setReadOnly();
    else
        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

CFormDateTimeField::~CFormDateTimeField()
{

}

QVariant CFormDateTimeField::getValue()
{
    return this->dateTime();
}

void CFormDateTimeField::setValue(QVariant value)
{
    if (value.type() == QVariant::DateTime)
       {
           setRawValue(value);
           this->setDateTime(value.toDateTime());
       }
    if (value.type() == QVariant::Date)
       {
           setRawValue(value);
           this->setDate(value.toDate());
       }
}

void CFormDateTimeField::setReadOnly()
{
    CDateTimeEdit::setReadOnly(true);
    CDateTimeEdit::setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(QString());
}

void CFormDateTimeField::setReadWrite()
{
    CDateTimeEdit::setReadOnly(false);
    CDateTimeEdit::setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

void CFormDateTimeField::select()
{
    this->selectAll();
}

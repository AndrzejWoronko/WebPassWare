#include "FormDateField.h"

CFormDateField::CFormDateField(const QString &variableName, const QString &label, const QDate &value, bool readOnly, QWidget *parent):
        CDateEdit(value, parent), CFormAbstractField(QMetaType::QDate, variableName, label, value)
{
    m_widget = this;
    if (readOnly)
        this->setReadOnly();
    else
        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

CFormDateField::~CFormDateField()
{

}

QVariant CFormDateField::getValue()
{
    return this->date();
}

void CFormDateField::setValue(QVariant value)
{
    if (value.userType() == QMetaType::QDate)
        {
            setRawValue(value);
            this->setDate(value.toDate());
        }
}

void CFormDateField::setReadOnly()
{
    CDateEdit::setReadOnly(true);
    CDateEdit::setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(QString());
}

void CFormDateField::setReadWrite()
{
    CDateEdit::setReadOnly(false);
    CDateEdit::setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

void CFormDateField::select()
{
    this->selectAll();
}

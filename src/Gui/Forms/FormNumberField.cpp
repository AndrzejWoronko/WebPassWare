#include "FormNumberField.h"

/***
Pole całkowite

UWAGA !!! Dla LongLong stosujemy CFormNumberField ze względu na większy zakres setMaximum(INT64_MAX);
UWAGA !!! Dla pola CFormNumberField z uwagi na precyzję nie można uzyskać wartości 0

***/
CFormNumberIntField::CFormNumberIntField(const QString &variableName, const QString &label, QVariant value, int minimum, bool readOnly,
                                   const QString &prefix, const QString &suffix, QWidget *parent) :
        QSpinBox(parent), CFormAbstractField(QVariant::Int, variableName, label, value)
{
    m_widget = this;
    this->setMinimum(minimum);
    this->setMaximum(INT32_MAX);
    this->setValue(value.toInt());
    this->setAlignment(Qt::AlignRight);

    if (readOnly)
        this->setReadOnly();
    else
        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);

    if (!prefix.isEmpty())
        this->setPrefix(prefix);
    if (!suffix.isEmpty())
        this->setSuffix(suffix);
}

CFormNumberIntField::~CFormNumberIntField()
{

}

QVariant CFormNumberIntField::getValue()
{
    return this->value();
}

void CFormNumberIntField::setValue(QVariant value)
{
    setRawValue(value);
    QSpinBox::setValue(value.toInt());
}

void CFormNumberIntField::setReadOnly()
{
    QSpinBox::setReadOnly(true);
    QSpinBox::setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(QString());
}

void CFormNumberIntField::setReadWrite()
{
    QSpinBox::setReadOnly(false);
    QSpinBox::setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

void CFormNumberIntField::select()
{
    this->selectAll();
}

void CFormNumberIntField::keyPressEvent(QKeyEvent *event)
{
  if (event)
  {
      switch(event->key())
      {
        case Qt::Key_F1:
            emit keyF1();
        break;
        case Qt::Key_F2:
            emit keyF2();
        break;
        case Qt::Key_F3:
            emit keyF3();
        break;
        case Qt::Key_F4:
            emit keyF4();
        break;
        case Qt::Key_F5:
            emit keyF5();
        break;
        case Qt::Key_F6:
            emit keyF6();
        break;
        case Qt::Key_F7:
            emit keyF7();
        break;
        case Qt::Key_F8:
            emit keyF8();
        break;
        case Qt::Key_F9:
            emit keyF9();
        break;
        case Qt::Key_F10:
            emit keyF10();
        break;
        case Qt::Key_F11:
            emit keyF11();
        break;
        case Qt::Key_F12:
            emit keyF12();
        break;
        default:
          //event->ignore();
          QSpinBox::keyPressEvent(event);
          break;
      }
  }
}

/***
Pole zmienno przecinkowe
***/

CFormNumberField::CFormNumberField(const QString &variableName, const QString &label, QVariant value, int precision, double minimum, bool readOnly,
                                   const QString &prefix, const QString &suffix, QWidget *parent) :
        QDoubleSpinBox(parent), CFormAbstractField(QVariant::Double, variableName, label, value)
{
    m_widget = this;
    if (precision)
         {
           double singleStep = static_cast<double>((1.0 / qPow(10.0, precision)));
           this->setSingleStep(singleStep);
         }
    else
        this->setSingleStep(1.00);

    this->setMinimum(minimum);    
    this->setMaximum(INT64_MAX);
    this->setDecimals(precision);
    this->setValue(value.toDouble());

    this->setAlignment(Qt::AlignRight);

    if (readOnly)
        this->setReadOnly();
    else
        this->setStyleSheet(FORM_FIELD_EDIT_STYLE);

    if (!prefix.isEmpty())
        this->setPrefix(prefix);
    if (!suffix.isEmpty())
        this->setSuffix(suffix);
}

CFormNumberField::~CFormNumberField()
{

}

QVariant CFormNumberField::getValue()
{
    return this->value();
}

void CFormNumberField::setValue(QVariant value)
{
    setRawValue(value);
    QDoubleSpinBox::setValue(value.toDouble());
}

void CFormNumberField::setReadOnly()
{
    QDoubleSpinBox::setReadOnly(true);
    QDoubleSpinBox::setFocusPolicy(Qt::NoFocus);        
    this->setStyleSheet(QString());
}

void CFormNumberField::setReadWrite()
{
    QDoubleSpinBox::setReadOnly(false);
    QDoubleSpinBox::setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

void CFormNumberField::select()
{
    this->selectAll();
}

void CFormNumberField::keyPressEvent(QKeyEvent *event)
{
  if (event)
  {
      switch(event->key())
      {
        case Qt::Key_F1:
            emit keyF1();
        break;
        case Qt::Key_F2:
            emit keyF2();
        break;
        case Qt::Key_F3:
            emit keyF3();
        break;
        case Qt::Key_F4:
            emit keyF4();
        break;
        case Qt::Key_F5:
            emit keyF5();
        break;
        case Qt::Key_F6:
            emit keyF6();
        break;
        case Qt::Key_F7:
            emit keyF7();
        break;
        case Qt::Key_F8:
            emit keyF8();
        break;
        case Qt::Key_F9:
            emit keyF9();
        break;
        case Qt::Key_F10:
            emit keyF10();
        break;
        case Qt::Key_F11:
            emit keyF11();
        break;
        case Qt::Key_F12:
            emit keyF12();
        break;
        default:
          //event->ignore();
          QDoubleSpinBox::keyPressEvent(event);
          break;
      }
  }
}

CFormPriceField::CFormPriceField(const QString &variableName, const QString &label, QVariant value, bool readOnly, QWidget *parent) :
                CFormNumberField(variableName, label, value, 2, 0.01, readOnly, QString(), tr(" zł"), parent)
{

}

CFormValueField::CFormValueField(const QString &variableName, const QString &label, QVariant value, bool readOnly, QWidget *parent) :
                CFormNumberField(variableName, label, value, 2, INT64_MIN, readOnly, QString(), tr(" zł"), parent)
{

}

CFormPercentField::CFormPercentField(const QString &variableName, const QString &label, QVariant value, bool readOnly, QWidget *parent) :
                CFormNumberField(variableName, label, value, 2, INT64_MIN, readOnly, QString(), tr(" %"), parent)
{

}

CFormIndicatorField::CFormIndicatorField(const QString &variableName, const QString &label, QVariant value, bool readOnly, QWidget *parent) :
                CFormNumberField(variableName, label, value, 4, INT64_MIN, readOnly, QString(), QString(), parent)
{

}

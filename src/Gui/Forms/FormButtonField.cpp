#include "FormButtonField.h"

  CFormButtonField::CFormButtonField(const QString &variableName, const QString &label, QVariant value, QWidget *parent):
    QPushButton(parent), CFormAbstractField(QVariant::String, variableName, label, value)
  {
    m_widget = this;
    setValue(value);
    connect(this, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
  }

  CFormButtonField::~CFormButtonField()
  {
  }

  QVariant CFormButtonField::getValue()
  {
    return m_value;
  }

  void CFormButtonField::setValue(QVariant value)
  {    
    setRawValue(value);
  }

  void CFormButtonField::onPushButtonClicked()
  {

  }

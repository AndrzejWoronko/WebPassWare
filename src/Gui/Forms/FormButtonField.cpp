#include "FormButtonField.h"

namespace wpw {

  CFormButtonField::CFormButtonField(const QString &variableName, const QString &label, QVariant value, QWidget *parent):
    QPushButton(parent), CFormAbstractField(QMetaType::QString, variableName, label, value)
  {
    m_widget = this;
    setValue(value);
    connect(this, &CFormButtonField::clicked, this, &CFormButtonField::onPushButtonClicked);
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

} // namespace wpw

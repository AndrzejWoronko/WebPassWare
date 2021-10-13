#ifndef FORMBUTTONFIELD_H
#define FORMBUTTONFIELD_H

#include "Global.h"
#include <QPushButton>
#include "FormAbstractField.h"

class CFormButtonField : public QPushButton, public CFormAbstractField
{
    Q_OBJECT
  public:

    CFormButtonField(const QString &variableName, const QString &label, QVariant value, QWidget *parent = NULL);
    ~CFormButtonField();

    virtual QVariant getValue();    
    virtual void setValue(QVariant value);

  public slots:

    virtual void onPushButtonClicked();
};

#endif // FORMBUTTONFIELD_H

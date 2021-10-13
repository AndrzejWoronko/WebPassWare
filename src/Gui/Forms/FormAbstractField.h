#ifndef FORMABSTRACTFIELD_H
#define FORMABSTRACTFIELD_H

#include "Global.h"
#include <QString>
#include <QVariant>
#include <QLabel>

#define FORM_FIELD_FG_EDIT_COLOR "#000000"
#define FORM_FIELD_BG_EDIT_COLOR "#ffffc8"
#define FORM_FIELD_BG_NOEDIT_COLOR "#ffffff"

#define FORM_FIELD_EDIT_STYLE "* {color: rgb( 0, 0, 0); background-color: rgb(255, 255, 200)}"
/**
   * @brief The CFormAbstractField class - bazowe pole formatki
*/
class CFormAbstractField
{   
    ADD_PTR_PRO(QWidget, m_widget, getWidget)

  public:
    //type, variable_name, label, value, validator, select_query
    CFormAbstractField(QVariant::Type type, const QString &variableName, const QString &label, QVariant value);
    virtual ~CFormAbstractField();

    virtual QVariant getValue(void) = 0;
    virtual void setValue(QVariant value) = 0;
    virtual QVariant getRawValue(void);
    void setRawValue(QVariant value);

    QVariant getStartValue(void);
    void setStartValue(QVariant value);

    QString getVariableName(void);
    QString getLabel(void);    

    virtual bool isDirty(void);
    virtual void select();

  protected:
    QVariant::Type m_type;
    QString m_variableName;
    QString m_label;
    QVariant m_value;
    QVariant m_startValue;    
};

typedef QList<CFormAbstractField*> FormFieldList;
typedef QString (*validateFunc)(FormFieldList); //Typ funkcji walidującej dane z QListy

typedef QHash<QString, CFormAbstractField*> FormFieldHashList;
typedef QString (*validateFuncHash)(FormFieldHashList); //Typ funkcji walidującej dane z listy QHash

#endif // FORMABSTRACTFIELD_H

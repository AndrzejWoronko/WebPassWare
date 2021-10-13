#ifndef FORMNUMBERFIELD_H
#define FORMNUMBERFIELD_H

#include "Global.h"
#include <QDoubleSpinBox>
#include "FormAbstractField.h"
#include <QKeyEvent>

class CFormNumberIntField : public QSpinBox, public CFormAbstractField
{
    Q_OBJECT

public:
    CFormNumberIntField(const QString &variableName, const QString &label, QVariant value, int minimum = 0, bool readOnly = false, const QString &prefix = QString(), const QString &suffix = QString(), QWidget *parent = NULL);
    ~CFormNumberIntField();

    QVariant getValue();
    virtual void setValue(QVariant value);
    void setReadOnly();
    void setReadWrite();

    virtual void select();

    void keyPressEvent(QKeyEvent *event);

signals:
    void keyF1();
    void keyF2();
    void keyF3();
    void keyF4();
    void keyF5();
    void keyF6();
    void keyF7();
    void keyF8();
    void keyF9();
    void keyF10();
    void keyF11();
    void keyF12();
};

class CFormNumberField : public QDoubleSpinBox, public CFormAbstractField
{
    Q_OBJECT

public:
    CFormNumberField(const QString &variableName, const QString &label, QVariant value, int precision = 0, double minimum = 0, bool readOnly = false, const QString &prefix = QString(), const QString &suffix = QString(), QWidget *parent = NULL);
    ~CFormNumberField();

    QVariant getValue();    
    virtual void setValue(QVariant value);
    void setReadOnly();
    void setReadWrite();

    virtual void select();

    void keyPressEvent(QKeyEvent *event);

signals:
    void keyF1();
    void keyF2();
    void keyF3();
    void keyF4();
    void keyF5();
    void keyF6();
    void keyF7();
    void keyF8();
    void keyF9();
    void keyF10();
    void keyF11();
    void keyF12();
};

//Szczegółowe typy pól do edycji

class CFormPriceField: public CFormNumberField
{
    Q_OBJECT

public:
    CFormPriceField(const QString &variableName, const QString &label, QVariant value, bool readOnly = false, QWidget *parent = NULL);
};

class CFormValueField: public CFormNumberField
{
    Q_OBJECT

public:
       CFormValueField(const QString &variableName, const QString &label, QVariant value, bool readOnly = false, QWidget *parent = NULL);
};

class CFormPercentField: public CFormNumberField
{
    Q_OBJECT

public:
       CFormPercentField(const QString &variableName, const QString &label, QVariant value, bool readOnly = false, QWidget *parent = NULL);
};

class CFormIndicatorField: public CFormNumberField
{
    Q_OBJECT

public:
       CFormIndicatorField(const QString &variableName, const QString &label, QVariant value, bool readOnly = false, QWidget *parent = NULL);
};

#endif // FORMNUMBERFIELD_H

#ifndef SQLFORMTEXTFIELD_H
#define SQLFORMTEXTFIELD_H

#include "Global.h"
#include "FixedLineEdit.h"
#include "FormAbstractField.h"
#include <QKeyEvent>


class CFormTextField : public CFixedLineEdit, public CFormAbstractField
{
    Q_OBJECT
public:
    CFormTextField(const QString &variableName, const QString &label, QString value, int lenght = 255, bool readOnly = false, QWidget *parent = NULL);
    ~CFormTextField();

    QVariant getValue();    
    virtual void setValue(QVariant value);

    void setPasswdEcho();
    void setReadOnly();
    void setReadWrite();

    virtual void select();

    void keyPressEvent(QKeyEvent *event);
private:

    int m_length;

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

#endif // SQLFORMTEXTFIELD_H

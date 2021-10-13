#ifndef FORMEDITORFIELD_H
#define FORMEDITORFIELD_H

#include "Global.h"
#include <QTextBrowser>
#include <QTextEdit>
#include <QKeyEvent>

#include "FormAbstractField.h"

class CFormEditorField : public QTextEdit, public CFormAbstractField
{
    Q_OBJECT
  public:
    CFormEditorField(const QString &variableName, const QString &label, QString value, bool readOnly = false, QWidget *parent = NULL);
    ~CFormEditorField();

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

#endif // FORMEDITORFIELD_H

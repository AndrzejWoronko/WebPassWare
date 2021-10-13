#include "FormEditorField.h"

CFormEditorField::CFormEditorField(const QString &variableName, const QString &label, QString value, bool readOnly, QWidget *parent) :
        QTextEdit(value, parent), CFormAbstractField(QVariant::String, variableName, label, value)
{
    m_widget = this;
    if (readOnly)
        this->setReadOnly();
    else
        {
            this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
            this->setPlaceholderText(QString(tr("Wpisz ")) + label.toLower());
        }
}

CFormEditorField::~CFormEditorField()
{

}

QVariant CFormEditorField::getValue()
{
    return this->toPlainText();
}

void CFormEditorField::setValue(QVariant value)
{
    setRawValue(value);
    this->setText(value.toString());
}

void CFormEditorField::setReadOnly()
{
    QTextEdit::setReadOnly(true);
    QTextEdit::setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(QString());
}

void CFormEditorField::setReadWrite()
{
    QTextEdit::setReadOnly(false);
    QTextEdit::setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(FORM_FIELD_EDIT_STYLE);
}

void CFormEditorField::select()
{
    this->selectAll();
}

void CFormEditorField::keyPressEvent(QKeyEvent *event)
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
          QTextEdit::keyPressEvent(event);
          break;
      }
  }
}

#include "FixedLineEdit.h"

CFixedLineEdit::CFixedLineEdit(const QString &text, QWidget *parent) : QLineEdit(text, parent)
{

}

CFixedLineEdit::CFixedLineEdit(const QString &text, int max, int min, QWidget *parent) : QLineEdit(text, parent)
{
   this->setMinVisibleChars(min);
   this->setMaxVisibleChars(max);
}

int CFixedLineEdit::getMinVisibleChars() const
{
    return m_minVisible;
}
int CFixedLineEdit::getMaxVisibleChars() const
{
    return m_maxVisible;
}

void CFixedLineEdit::setMinVisibleChars(int count)
{
    m_minVisible = count;
    setMinimumWidth(this->fontMetrics().horizontalAdvance('W') * m_minVisible +
        ((this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth) + 2) * 2));
}

void CFixedLineEdit::setMaxVisibleChars(int count)
{
    m_maxVisible = count;
    setMaximumWidth(this->fontMetrics().horizontalAdvance('W') * m_maxVisible +
        ((this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth) + 2) * 2));
}

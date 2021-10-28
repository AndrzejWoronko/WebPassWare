#include "Label.h"

CLabel::CLabel(const QString &text, const QString &pixmap, QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f)
{
    m_text = text;

    if (!m_text.isEmpty())
        setText(m_text);
    this->setPixmap(pixmap);
}

void CLabel::setPixmap(const QString &pixmap)
{
    if (!pixmap.isEmpty())
    {
        if (CStyle::iconFromStyleExist(pixmap))
            QLabel::setPixmap(CStyle::pixmapFromStyle(pixmap));
    }
}

void CLabel::setBold(void)
{
    if (!m_text.isEmpty())
        setText(QString("<b>%1</b>").arg(m_text));
}

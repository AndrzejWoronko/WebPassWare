#include "FontEdit.h"
#include <QFontDialog>

CFontEdit::CFontEdit(QWidget *parent) : QWidget(parent)
{

    m_layout = new CHBoxLayout(this);
    m_label = new QLabel(this);
    m_button = new QToolButton(this);
    m_button->setText(QString("..."));

    if (CStyle::iconFromStyleExist(QString("Font")))
        m_button->setIcon(CStyle::iconFromStyle(ICON("Font")));
    else
        m_button->setIcon(QIcon::fromTheme(QString("font")));

    m_layout->addWidget(m_label);
    m_layout->addWidget(m_button);
    initConnections();
}

CFontEdit::~CFontEdit()
    {
        safe_delete(m_layout)
        safe_delete(m_label)
        safe_delete(m_button)
    }

QFont CFontEdit::getFont() const
{
    return m_font;
}

void CFontEdit::setFont(const QFont &arg)
{
    m_font = arg;
    updateFont();
}

void CFontEdit::initConnections()
{
    connect(m_button, SIGNAL(clicked()), this, SLOT(browse()));
    updateFont();
}

void CFontEdit::updateFont()
{
    m_label->setFont(m_font);
    int size = m_font.pointSize() > -1 ? m_font.pointSize() : m_font.pixelSize();
    m_label->setText(QString("%1, %2").arg(m_font.family()).arg(size));
}

void CFontEdit::browse()
{
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, m_label->font(), this, tr("Wybierz czcionkę"));
        if (!ok)
        return;
    m_font = newFont;
    updateFont();
    emit fontChanged(newFont);
}

void CFontEdit::setToolTip(const QString &text)
{
    if (m_button)
        m_button->setToolTip(text);
}

void CFontEdit::setFontSize(int size)
{
    m_font.setPointSize(size);
    updateFont();
    emit fontSizeChanged(size);
}

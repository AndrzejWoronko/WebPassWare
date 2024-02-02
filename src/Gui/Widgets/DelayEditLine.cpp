#include "DelayEditLine.h"

CDelayEditLine::CDelayEditLine(QWidget *parent) : QLineEdit(parent)
{
    this->setClearButtonEnabled(true);

    m_typingTimer.reset(new QTimer(this));
    m_typingTimer->setSingleShot(true);
    connect(this, &QLineEdit::textChanged, this, &CDelayEditLine::onTextEdited);
    connect(m_typingTimer.get(), &QTimer::timeout, this, &CDelayEditLine::delayEditingFinish);
}

// CDelayEditLine::~CDelayEditLine()
// {
//     //qobject_safe_delete(m_typingTimer)
// }

void CDelayEditLine::onTextEdited(const QString & newText)
{
    disconnect(this, &QLineEdit::textChanged, this, &CDelayEditLine::onTextEdited);
    this->setText(newText);
    connect(this, &QLineEdit::textChanged, this, &CDelayEditLine::onTextEdited);
    m_typingTimer->start(DELAY_EDIT_LINE_MS); // This will fire filterEntries after 300 ms.
}

void CDelayEditLine::delayEditingFinish()
{
    emit delayEditingFinished(this->text());
    DEBUG_WITH_LINE << "Text:" << this->text();
}

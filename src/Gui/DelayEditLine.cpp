#include "DelayEditLine.h"

CDelayEditLine::CDelayEditLine(QWidget *parent) : QLineEdit(parent)
{
    this->setClearButtonEnabled(true);

    m_typingTimer = new QTimer(this);
    m_typingTimer->setSingleShot(true);
    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(onTextEdited(const QString &)));
    connect(m_typingTimer, SIGNAL(timeout()), this, SLOT(delayEditingFinish()));
}

CDelayEditLine::~CDelayEditLine()
{
    qobject_safe_delete(m_typingTimer)
}

void CDelayEditLine::onTextEdited( const QString & newText)
{
    disconnect(this, SIGNAL(textChanged(const QString &)), this, SLOT(onTextEdited(const QString &)));
    this->setText(newText);
    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(onTextEdited(const QString &)));
    m_typingTimer->start(DELAY_EDIT_LINE_MS); // This will fire filterEntries after 100 ms.
}

void CDelayEditLine::delayEditingFinish()
{
    emit delayEditingFinished(this->text());
    DEBUG_WITH_LINE << "Text:" << this->text();
}

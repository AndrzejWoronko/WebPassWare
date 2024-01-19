#include "AutoTypeAction.h"
#include "Tools.h"

CAutoTypeChar::CAutoTypeChar(const QChar& character)
    : m_character(character)
{
}

CAutoTypeAction* CAutoTypeChar::clone()
{
    return new CAutoTypeChar(m_character);
}

void CAutoTypeChar::accept(CAutoTypeExecutor* executor)
{
    executor->execChar(this);
}


CAutoTypeKey::CAutoTypeKey(Qt::Key key)
    : m_key(key)
{
}

CAutoTypeAction* CAutoTypeKey::clone()
{
    return new CAutoTypeKey(m_key);
}

void CAutoTypeKey::accept(CAutoTypeExecutor* executor)
{
    executor->execKey(this);
}


CAutoTypeDelay::CAutoTypeDelay(int delayMs)
    : m_delayMs(delayMs)
{
}

CAutoTypeAction* CAutoTypeDelay::clone()
{
    return new CAutoTypeDelay(m_delayMs);
}

void CAutoTypeDelay::accept(CAutoTypeExecutor* executor)
{
    executor->execDelay(this);
}

void CAutoTypeExecutor::execDelay(CAutoTypeDelay* action)
{
    Tools::wait(action->m_delayMs);
}

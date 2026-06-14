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


CAutoTypeSetDelay::CAutoTypeSetDelay(int delayMs)
    : m_delayMs(delayMs)
{
}

CAutoTypeAction* CAutoTypeSetDelay::clone()
{
    return new CAutoTypeSetDelay(m_delayMs);
}

void CAutoTypeSetDelay::accept(CAutoTypeExecutor* executor)
{
    executor->execSetDelay(this);
}


CAutoTypeExecutor::CAutoTypeExecutor()
    : m_delay(10)
{
}

void CAutoTypeExecutor::execDelay(CAutoTypeDelay* action)
{
    Tools::wait(action->m_delayMs);
}

void CAutoTypeExecutor::execSetDelay(CAutoTypeSetDelay* action)
{
    m_delay = action->m_delayMs;
}

int CAutoTypeExecutor::delay() const
{
    return m_delay;
}

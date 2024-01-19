#ifndef CAUTOTYPEACTION_H
#define CAUTOTYPEACTION_H

#include <QChar>
#include <Qt>

class CAutoTypeExecutor;

class CAutoTypeAction
{
public:
    virtual ~CAutoTypeAction() {}
    virtual CAutoTypeAction* clone() = 0;
    virtual void accept(CAutoTypeExecutor* executor) = 0;
};

class CAutoTypeChar : public CAutoTypeAction
{
public:
    explicit CAutoTypeChar(const QChar& character);
    CAutoTypeAction* clone();
    void accept(CAutoTypeExecutor* executor);

    const QChar m_character;
};

class CAutoTypeKey : public CAutoTypeAction
{
public:
    explicit CAutoTypeKey(Qt::Key key);
    CAutoTypeAction* clone();
    void accept(CAutoTypeExecutor* executor);

    const Qt::Key m_key;
};

class CAutoTypeDelay : public CAutoTypeAction
{
public:
    explicit CAutoTypeDelay(int delayMs);
    CAutoTypeAction* clone();
    void accept(CAutoTypeExecutor* executor);

    const int m_delayMs;
};

class CAutoTypeExecutor
{
public:
    virtual ~CAutoTypeExecutor() {}
    virtual void execChar(CAutoTypeChar* action) = 0;
    virtual void execKey(CAutoTypeKey* action) = 0;
    virtual void execDelay(CAutoTypeDelay* action);
};


#endif // CAUTOTYPEACTION_H

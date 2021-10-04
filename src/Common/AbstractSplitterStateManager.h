#ifndef CABSTRACTSPLITTERSTATEMANAGER_H
#define CABSTRACTSPLITTERSTATEMANAGER_H

#include "SplitterState.h"

class CAbstractSplitterStateManager
{

protected:
    CSplitterState *m_splitterState;

public:
    CAbstractSplitterStateManager();
    ~CAbstractSplitterStateManager();

    virtual void restoreSplitterState() = 0;
    virtual void saveSplitterState() = 0;
};

#endif // CABSTRACTSPLITTERSTATEMANAGER_H

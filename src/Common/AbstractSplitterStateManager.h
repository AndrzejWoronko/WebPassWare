#ifndef CABSTRACTSPLITTERSTATEMANAGER_H
#define CABSTRACTSPLITTERSTATEMANAGER_H

#include "SplitterState.h"

class CAbstractSplitterStateManager
{

    ADD_QSMART_PTR(ICState, m_splitterState, getSplitterState)

public:
    CAbstractSplitterStateManager();
    ~CAbstractSplitterStateManager() = default;

    virtual void restoreSplitterState() = 0;
    virtual void saveSplitterState() = 0;
};

#endif // CABSTRACTSPLITTERSTATEMANAGER_H

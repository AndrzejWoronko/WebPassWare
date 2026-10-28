#ifndef CABSTRACTSPLITTERSTATEMANAGER_H
#define CABSTRACTSPLITTERSTATEMANAGER_H

#include "SplitterState.h"

namespace wpw {

class ICSplitterStateManager
{
public:
    ICSplitterStateManager() = default;
    virtual ~ICSplitterStateManager() = default;

    virtual void restoreSplitterState() = 0;
    virtual void saveSplitterState() = 0;
};

class CAbstractSplitterStateManager : public ICSplitterStateManager
{

    ADD_QSMART_UNIQUE_PTR(ICState, m_splitterState, getSplitterState)

public:
    CAbstractSplitterStateManager();
    ~CAbstractSplitterStateManager() = default;
};

} // namespace wpw

#endif // CABSTRACTSPLITTERSTATEMANAGER_H

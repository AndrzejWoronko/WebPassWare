#include "BusyIndicator.h"

#include <utility>

namespace wpw {

namespace {

BusyIndicator::Hook &beginHook()
{
    static BusyIndicator::Hook hook;
    return hook;
}

BusyIndicator::Hook &endHook()
{
    static BusyIndicator::Hook hook;
    return hook;
}

} // namespace

void BusyIndicator::setHooks(Hook begin, Hook end)
{
    beginHook() = std::move(begin);
    endHook() = std::move(end);
}

void BusyIndicator::begin()
{
    if (beginHook())
    {
        beginHook()();
    }
}

void BusyIndicator::end()
{
    if (endHook())
    {
        endHook()();
    }
}

} // namespace wpw

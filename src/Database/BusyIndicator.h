#ifndef BUSYINDICATOR_H
#define BUSYINDICATOR_H

#include <functional>

namespace wpw {

// Marks a running database operation. The data layer has no GUI: the
// application installs hooks that show a busy mouse cursor, the tests and
// tools without a GUI leave them empty.
class BusyIndicator
{
public:
    using Hook = std::function<void()>;

    static void setHooks(Hook begin, Hook end);
    static void begin();
    static void end();

    // begin() on construction, end() on destruction.
    class Scope
    {
    public:
        Scope() { BusyIndicator::begin(); }
        ~Scope() { BusyIndicator::end(); }
        Scope(const Scope &) = delete;
        Scope &operator=(const Scope &) = delete;
    };
};

} // namespace wpw

#endif // BUSYINDICATOR_H

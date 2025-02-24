#ifndef JUMI_GEO_DEBUG_TIMER_H
#define JUMI_GEO_DEBUG_TIMER_H
#include "typedefs.h"
#include <chrono>
#include <string>

NAMESPACE_BEGIN(geo)

class console_io;

class debug_timer
{
public:
    debug_timer(const std::string& name, console_io* io);
    ~debug_timer();

private:
    std::string _name;
    console_io* _io;
    std::chrono::high_resolution_clock::time_point _start;
};

NAMESPACE_END

#endif

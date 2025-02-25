#ifndef JUMI_GEO_DEBUG_TIMER_H
#define JUMI_GEO_DEBUG_TIMER_H
#include "typedefs.h"
#include <chrono>
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(geo)

class console_io;

extern std::unordered_map<std::string, long> execution_times_us;
extern std::string format_execution_time(const std::string& name, long micro_duration);

class debug_timer
{
public:
    debug_timer(const std::string& name);
    void stop();

private:
    std::string _name;
    std::chrono::high_resolution_clock::time_point _start;
};

NAMESPACE_END

#endif

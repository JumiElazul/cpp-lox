#include "debug_timer.h"
#include "typedefs.h"
#include <chrono>
#include <string>

NAMESPACE_BEGIN(geo)

std::unordered_map<std::string, long> execution_times_us = {};

std::string format_execution_time(const std::string& name, long micro_duration)
{
    return name + " took [ " + std::to_string(micro_duration) + "us | " + std::to_string(micro_duration / 1000) + "ms ] us\n";
}

debug_timer::debug_timer(const std::string& name)
    : _name(name)
    , _start(std::chrono::high_resolution_clock::now())
{

}

void debug_timer::stop()
{
    auto end = std::chrono::high_resolution_clock::now();
    long micro_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count();
    execution_times_us[_name] = micro_duration;
}

NAMESPACE_END

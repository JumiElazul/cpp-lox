#include "debug_timer.h"
#include "console_io.h"
#include "typedefs.h"
#include <chrono>
#include <string>

NAMESPACE_BEGIN(geo)

debug_timer::debug_timer(const std::string& name, console_io* io)
    : _name(name)
    , _io(io)
    , _start(std::chrono::high_resolution_clock::now())
{

}

debug_timer::~debug_timer()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto micro_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count();
    auto milli_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count();
    _io->out() << _name << " took [ " << micro_duration << "us | " << milli_duration << "ms ]" << " of execution time\n";
}

NAMESPACE_END

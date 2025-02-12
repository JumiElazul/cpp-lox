#include "geo_native_funcs.h"
#include "console_io.h"
#include <chrono>
#include <string>
#include <vector>

NAMESPACE_BEGIN(geo)

int clock::arity() { return 0; }
std::string clock::to_string() const { return "<native_func>clock"; }

literal_value clock::call(interpreter& i, const std::vector<literal_value>& args)
{
    auto now = std::chrono::system_clock::now();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    return static_cast<double>(ms_since_epoch) / 1000.0;
}

print::print(console_io* io)
    : _io(io) { }

int print::arity() { return 1; }
std::string print::to_string() const { return "<native_func>print"; }

literal_value print::call(interpreter& i, const std::vector<literal_value>& args)
{
    for (const auto& arg : args)
    {
        _io->out() << literal_tostr(arg) << ' ';
    }
    _io->out() << '\n';
    return std::monostate();
}

NAMESPACE_END

#include "console_io.h"
#include "typedefs.h"
#include <linenoise.hpp>
#include <iostream>
#include <memory>

NAMESPACE_BEGIN(cpplox)

struct console_io::console_io_impl
{
public:
    console_io_impl(std::ostream& os = std::cout, std::ostream& err_os = std::cerr)
        : _os(os)
        , _err_os(err_os)
    {
        linenoise::SetHistoryMaxLen(MAX_HISTORY_SIZE);
    }

    std::string readline(const char* msg) const
    {
        std::string return_value = linenoise::Readline(msg);
        linenoise::AddHistory(return_value.c_str());
        return return_value;
    }

    std::ostream& out() { return _os; }
    std::ostream& err() { return _err_os; }

private:
    std::ostream& _os;
    std::ostream& _err_os;
};

console_io::console_io() : _impl(std::make_unique<console_io_impl>()) {}
console_io::~console_io() = default;

std::string console_io::readline(const char* msg) const
{
    return std::string(_impl->readline(msg));
}

std::ostream& console_io::out() { return _impl->out(); }
std::ostream& console_io::err() { return _impl->err(); }

NAMESPACE_END

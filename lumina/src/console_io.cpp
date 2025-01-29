#include "console_io.h"
#include "typedefs.h"
#include <iostream>
#include <memory>
#include <string>

NAMESPACE_BEGIN(lumina)

struct console_io::console_io_impl
{
public:
    console_io_impl(std::ostream& os = std::cout, std::ostream& err_os = std::cerr)
        : _os(os)
        , _err_os(err_os) { }

    std::string readline() const
    {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    std::ostream& out() { return _os; }
    std::ostream& err() { return _err_os; }

private:
    std::ostream& _os;
    std::ostream& _err_os;
};

console_io::console_io() : _impl(std::make_unique<console_io_impl>()) {}
console_io::~console_io() = default;

std::string console_io::readline() const
{
    return _impl->readline();
}

std::ostream& console_io::out() { return _impl->out(); }
std::ostream& console_io::err() { return _impl->err(); }

NAMESPACE_END

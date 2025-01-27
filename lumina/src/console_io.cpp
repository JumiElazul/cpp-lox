#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <iostream>
#include <initializer_list>
#include <string>

NAMESPACE_BEGIN(lumina)

console_io::console_io(std::ostream& os) : _os(os) { }

void console_io::write(const std::string& message) const
{
    std::cout << message;
}

void console_io::write(const std::initializer_list<std::string>& messages) const
{
    for (const auto& m : messages)
        std::cout << m;
}

std::string console_io::readline() const
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

console_io& console_io::operator<<(const char c)
{
    _os << c;
    return *this;
}

console_io& console_io::operator<<(const std::string& message)
{
    _os << message;
    return *this;
}

console_io& console_io::operator<<(const token& t)
{
    _os << t;
    return *this;
}

NAMESPACE_END

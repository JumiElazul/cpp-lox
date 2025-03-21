#include "exceptions.h"
#include "typedefs.h"
#include <stdexcept>
#include <string>
#include <sstream>

NAMESPACE_BEGIN(cpplox)

cpplox_runtime_error::cpplox_runtime_error(const std::string& msg)
    : std::runtime_error(msg)
{
    std::stringstream ss;
    ss << std::runtime_error::what();
    _msg = ss.str();
}

cpplox_runtime_error::cpplox_runtime_error(const std::string& msg, const token& t)
    : std::runtime_error(msg)
{
    std::stringstream ss;
    ss << std::runtime_error::what();
    ss << get_token_position(t);
    _msg = ss.str();
}

const char* cpplox_runtime_error::what() const noexcept
{
    return _msg.c_str();
}

cpplox_type_error::cpplox_type_error(const std::string& msg, const token& t)
    : cpplox_runtime_error(msg, t) { }

std::string get_token_position(const token& t)
{
    std::stringstream ss;

    if (t.type == token_type::eof_)
    {
        ss << " at 'end of file'";
        return ss.str();
    }

    ss << " on line/col [" << t.position.first << ":" << t.position.second << "] on token '" << t.lexeme << "'";
    ss << '\n';
    ss << ">>> " << t.source_line;
    return ss.str();
}

NAMESPACE_END

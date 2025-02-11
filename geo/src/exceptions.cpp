#include "exceptions.h"
#include "typedefs.h"
#include <stdexcept>
#include <string>
#include <sstream>

NAMESPACE_BEGIN(geo)

geo_runtime_error::geo_runtime_error(const std::string& msg)
    : std::runtime_error(msg)
{
    std::stringstream ss;
    ss << std::runtime_error::what();
    _msg = ss.str();
}

geo_runtime_error::geo_runtime_error(const std::string& msg, const token& t)
    : std::runtime_error(msg)
{
    std::stringstream ss;
    ss << std::runtime_error::what();
    ss << get_token_position(t);
    _msg = ss.str();
}

const char* geo_runtime_error::what() const noexcept
{
    return _msg.c_str();
}

geo_type_error::geo_type_error(const std::string& msg, const token& t)
    : geo_runtime_error(msg, t) { }

std::string get_token_position(const token& t)
{
    std::stringstream ss;

    if (t.type == token_type::eof_)
    {
        ss << " at 'end of file'";
        return ss.str();
    }

    ss << " on line/col [" << t.position.first << ":" << t.position.second << "] on token '" << t.lexeme << "'";
    return ss.str();
}

NAMESPACE_END

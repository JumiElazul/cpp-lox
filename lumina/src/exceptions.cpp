#include "exceptions.h"
#include "typedefs.h"
#include <stdexcept>
#include <string>
#include <sstream>

NAMESPACE_BEGIN(lumina)

lumina_runtime_error::lumina_runtime_error(const std::string& msg)
    : std::runtime_error(msg)
{
    std::stringstream ss;
    ss << std::runtime_error::what();
    _msg = ss.str();
}

const char* lumina_runtime_error::what() const noexcept
{
    return _msg.c_str();
}

lumina_parse_error::lumina_parse_error(const std::string& msg, const token& t)
    : lumina_runtime_error(msg)
    , tok(t)
{ 
    _msg += get_token_position(t);
}

lumina_type_error::lumina_type_error(const std::string& msg, const token& t)
    : lumina_runtime_error(msg)
    , tok(t)
{ 
    _msg += get_token_position(t);
}

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

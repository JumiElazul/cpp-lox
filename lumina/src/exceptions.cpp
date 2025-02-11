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

lumina_runtime_error::lumina_runtime_error(const std::string& msg, const token& t)
    : std::runtime_error(msg)
{
    std::stringstream ss;
    ss << std::runtime_error::what();
    ss << get_token_position(t);
    _msg = ss.str();
}

const char* lumina_runtime_error::what() const noexcept
{
    return _msg.c_str();
}

lumina_type_error::lumina_type_error(const std::string& msg, const token& t)
    : lumina_runtime_error(msg, t) { }

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

lumina_loop_break::lumina_loop_break(const std::string& msg)
    : lumina_runtime_error(msg) { }

lumina_loop_continue::lumina_loop_continue(const std::string& msg)
    : lumina_runtime_error(msg) { }

NAMESPACE_END

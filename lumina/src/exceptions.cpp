#include "exceptions.h"
#include "typedefs.h"
#include <stdexcept>
#include <string>
#include <sstream>

NAMESPACE_BEGIN(lumina)

lumina_runtime_error::lumina_runtime_error(const std::string& msg)
    : std::runtime_error(msg)
{
}

const char* lumina_runtime_error::what() const noexcept
{
    return std::runtime_error::what();
}

lexer_error::lexer_error(const std::string& msg)
    : lumina_runtime_error(msg)
{
}

parser_error::parser_error(const std::string& msg, const token& t)
    : lumina_runtime_error(msg)
    , tok(std::move(t))
{

}

lumina_type_error::lumina_type_error(const std::string& msg, const token& t)
    : lumina_runtime_error(msg + get_token_position(t))
    , tok(std::move(t))
{ }

std::string get_token_position(const token& t)
{
    std::stringstream ss;
    ss << " at position [" << t.position.first << ":" << t.position.second << "] (" << t.lexeme << ")";
    return ss.str();
}

NAMESPACE_END

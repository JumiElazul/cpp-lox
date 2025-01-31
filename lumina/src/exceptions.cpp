#include "exceptions.h"
#include "typedefs.h"
#include <stdexcept>
#include <string>
#include <sstream>

NAMESPACE_BEGIN(lumina)

lexer_exception::lexer_exception(const std::string& message)
    : std::runtime_error(message)
{
}

const char* lexer_exception::what() const noexcept
{
    return std::runtime_error::what();
}

parser_exception::parser_exception(uint32 line, uint32 pos, const std::string& msg)
    : std::runtime_error(create_msg(line, pos, msg))
{
}

std::string parser_exception::create_msg(uint32 line, uint32 pos, const std::string& msg)
{
    std::ostringstream oss;
    oss << "error at line [" << line << ":" << pos << "]: "<< msg;
    return oss.str();
}

interpreter_exception::interpreter_exception(const std::string& message)
    : std::runtime_error(message)
{
}

const char* interpreter_exception::what() const noexcept
{
    return std::runtime_error::what();
}

NAMESPACE_END

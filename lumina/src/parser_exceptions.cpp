#include "parser_exceptions.h"
#include "typedefs.h"
#include <stdexcept>
#include <string>
#include <sstream>

NAMESPACE_BEGIN(lumina)

parser_exception::parser_exception(uint32 line, const std::string& msg)
    : std::runtime_error(create_msg(line, msg))
{
}

std::string parser_exception::create_msg(uint32 line, const std::string& msg)
{
    std::ostringstream oss;
    oss << "error at line " << line << ": " << msg;
    return oss.str();
}

NAMESPACE_END

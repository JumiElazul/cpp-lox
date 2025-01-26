#include "lexer_exceptions.h"
#include "typedefs.h"

NAMESPACE_BEGIN(lumina)

lexer_exception::lexer_exception(const std::string& message)
    : std::runtime_error(message)
{
}

const char* lexer_exception::what() const noexcept
{
    return std::runtime_error::what();
}

NAMESPACE_END

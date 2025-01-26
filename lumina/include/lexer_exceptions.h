#ifndef JUMI_LUMINA_LEXER_EXCEPTIONS_H
#define JUMI_LUMINA_LEXER_EXCEPTIONS_H
#include "typedefs.h"
#include <stdexcept>

NAMESPACE_BEGIN(lumina)

class lexer_exception : public std::runtime_error
{
public:
    lexer_exception(const std::string& message);
    virtual const char* what() const noexcept override;
};

NAMESPACE_END

#endif

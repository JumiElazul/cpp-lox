#ifndef JUMI_LUMINA_EXCEPTIONS_H
#define JUMI_LUMINA_EXCEPTIONS_H
#include "typedefs.h"
#include "tokens.h"
#include <stdexcept>
#include <string>

NAMESPACE_BEGIN(lumina)

class lumina_runtime_error : public std::runtime_error
{
public:
    lumina_runtime_error(const std::string& msg);
    virtual const char* what() const noexcept override;

};

class lexer_error : public lumina_runtime_error
{
public:
    lexer_error(const std::string& msg);
};

class parser_error : public lumina_runtime_error
{
public:
    token tok;
    parser_error(const std::string& msg, const token& t);
};

class lumina_type_error : public lumina_runtime_error
{
public:
    token tok;
    lumina_type_error(const std::string& msg, const token& t);
};

extern std::string get_token_position(const token& t);

NAMESPACE_END

#endif

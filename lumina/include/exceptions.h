#ifndef JUMI_LUMINA_EXCEPTIONS_H
#define JUMI_LUMINA_EXCEPTIONS_H
#include "typedefs.h"
#include <stdexcept>

NAMESPACE_BEGIN(lumina)

class lexer_exception : public std::runtime_error
{
public:
    lexer_exception(const std::string& message);
    virtual const char* what() const noexcept override;
};

class parser_exception : public std::runtime_error
{
public:
    uint32 line;
    uint32 pos;
    parser_exception(uint32 line, uint32 pos, const std::string& msg);

private:
    static std::string create_msg(uint32 line, uint32 pos, const std::string& msg);
};

class interpreter_exception : public std::runtime_error
{
public:
    interpreter_exception(const std::string& message);
    virtual const char* what() const noexcept override;
};

NAMESPACE_END

#endif

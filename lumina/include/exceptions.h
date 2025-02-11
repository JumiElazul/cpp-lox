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
    lumina_runtime_error(const std::string& msg, const token& t);
    virtual const char* what() const noexcept override;

protected:
    std::string _msg;
};

class lumina_type_error : public lumina_runtime_error
{
public:
    lumina_type_error(const std::string& msg, const token& t);
};

class lumina_loop_break : public lumina_runtime_error
{
public:
    lumina_loop_break(const std::string& msg);
};

class lumina_loop_continue : public lumina_runtime_error
{
public:
    lumina_loop_continue(const std::string& msg);
};

extern std::string get_token_position(const token& t);

NAMESPACE_END

#endif

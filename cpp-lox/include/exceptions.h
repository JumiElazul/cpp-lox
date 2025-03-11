#ifndef JUMI_CPPLOX_EXCEPTIONS_H
#define JUMI_CPPLOX_EXCEPTIONS_H
#include "typedefs.h"
#include "tokens.h"
#include <stdexcept>
#include <string>

NAMESPACE_BEGIN(cpplox)

class cpplox_runtime_error : public std::runtime_error
{
public:
    cpplox_runtime_error(const std::string& msg);
    cpplox_runtime_error(const std::string& msg, const token& t);
    virtual const char* what() const noexcept override;

protected:
    std::string _msg;
};

class cpplox_type_error : public cpplox_runtime_error
{
public:
    cpplox_type_error(const std::string& msg, const token& t);
};

extern std::string get_token_position(const token& t);

NAMESPACE_END

#endif

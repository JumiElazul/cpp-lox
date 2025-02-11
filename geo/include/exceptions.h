#ifndef JUMI_GEO_EXCEPTIONS_H
#define JUMI_GEO_EXCEPTIONS_H
#include "typedefs.h"
#include "tokens.h"
#include <stdexcept>
#include <string>

NAMESPACE_BEGIN(geo)

class geo_runtime_error : public std::runtime_error
{
public:
    geo_runtime_error(const std::string& msg);
    geo_runtime_error(const std::string& msg, const token& t);
    virtual const char* what() const noexcept override;

protected:
    std::string _msg;
};

class geo_type_error : public geo_runtime_error
{
public:
    geo_type_error(const std::string& msg, const token& t);
};

extern std::string get_token_position(const token& t);

NAMESPACE_END

#endif

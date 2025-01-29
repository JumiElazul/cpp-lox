#ifndef JUMI_LUMINA_PARSER_EXCEPTIONS_H
#define JUMI_LUMINA_PARSER_EXCEPTIONS_H
#include "typedefs.h"
#include <stdexcept>

NAMESPACE_BEGIN(lumina)

class parser_exception : public std::runtime_error
{
public:
    uint32 line;
    parser_exception(uint32 line, const std::string& msg);

private:
    static std::string create_msg(uint32 line, const std::string& msg);
};

NAMESPACE_END

#endif

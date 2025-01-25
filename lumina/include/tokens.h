#ifndef JUMI_LUMINA_TOKENS_H
#define JUMI_LUMINA_TOKENS_H
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(lumina)
using literal_value = std::variant<double, std::string, std::monostate>;

enum class token_type
{

};

struct token
{
    token_type type;
    literal_value literal;
};

NAMESPACE_END

#endif

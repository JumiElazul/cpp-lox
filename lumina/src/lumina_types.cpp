#include "lumina_types.h"
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(lumina)

std::string literal_tostr(const literal_value& l)
{
    return std::visit(
        literal_value_overload{
            [](double d)             { return std::to_string(d);                              },
            [](bool b)               { return b ? std::string("true") : std::string("false"); },
            [](const std::string& s) { return s;                                              },
            [](std::monostate)       { return std::string("null");                            },
        }, l
    );
}

NAMESPACE_END

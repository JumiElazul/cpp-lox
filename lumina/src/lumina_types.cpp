#include "lumina_types.h"
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(lumina)

lumina_type literal_to_lumina_type(const literal_value& l)
{
    if (l.index() < static_cast<int>(lumina_type::invalid_))
        return static_cast<lumina_type>(l.index());

    return lumina_type::invalid_;
}

std::string lumina_type_tostr(lumina_type type)
{
    switch (type)
    {
        case lumina_type::string_:
            return "string";
        case lumina_type::number_:
            return "number";
        case lumina_type::bool_:
            return "bool";
        case lumina_type::null_:
            return "null";
        default:
            return "invalid";
    }
}

std::string literal_tostr(const literal_value& l)
{
    auto format_number = [](double d) -> std::string {
        std::string s = std::to_string(d);
        size_t i = s.find_last_not_of('0');
        if (s[i] == '.')
            return s.substr(0, i);

        return s.substr(0, i + 1);
    };

    return std::visit(
        literal_value_overload{
            [&](double d)             { return format_number(d);                               },
            [&](bool b)               { return b ? std::string("true") : std::string("false"); },
            [&](const std::string& s) { return s;                                              },
            [&](std::monostate)       { return std::string("null");                            },
        }, l);
}

NAMESPACE_END

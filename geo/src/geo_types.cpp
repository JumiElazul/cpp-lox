#include "geo_types.h"
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(geo)

geo_type literal_to_geo_type(const literal_value& l)
{
    if (l.index() < static_cast<int>(geo_type::invalid_))
        return static_cast<geo_type>(l.index());

    return geo_type::invalid_;
}

std::string geo_type_tostr(geo_type type)
{
    switch (type)
    {
        case geo_type::string_:
            return "string";
        case geo_type::number_:
            return "number";
        case geo_type::bool_:
            return "bool";
        case geo_type::null_:
            return "null";
        case geo_type::callable_:
            return "callable";
        case geo_type::undefined_:
            return "undefined";
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
            [&](double d)                    { return format_number(d);                               },
            [&](bool b)                      { return b ? std::string("true") : std::string("false"); },
            [&](const std::string& s)        { return s;                                              },
            [&](std::monostate)              { return std::string("null");                            },
            [&](const undefined& u)          { return std::string("undefined");                       },
            [&](geo_callable*)               { return std::string("callable");                        },
        }, l);
}

NAMESPACE_END

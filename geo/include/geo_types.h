#ifndef JUMI_GEO_GEO_TYPES_H
#define JUMI_GEO_GEO_TYPES_H
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(geo)

struct undefined
{
    bool operator==(const undefined&) const { return true; }
    bool operator!=(const undefined&) const { return false; }
};

class geo_callable;
using literal_value = std::variant<double, bool, std::string, std::monostate, geo_callable*, undefined>;

enum class geo_type
{
    number_,
    bool_,
    string_,
    null_,
    callable_,
    undefined_,

    invalid_,
};

geo_type literal_to_geo_type(const literal_value& l);
std::string geo_type_tostr(geo_type type);

template<typename... Ts>
struct literal_value_overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
literal_value_overload(Ts...) -> literal_value_overload<Ts...>;

extern std::string literal_tostr(const literal_value& l);

NAMESPACE_END

#endif

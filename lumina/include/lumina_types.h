#ifndef JUMI_LUMINA_LUMINA_TYPES_H
#define JUMI_LUMINA_LUMINA_TYPES_H
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(lumina)

using literal_value = std::variant<double, bool, std::string, std::monostate>;

enum class lumina_type
{
    number_,
    bool_,
    string_,
    null_,
    undefined_,
    invalid_,
};

lumina_type literal_to_lumina_type(const literal_value& l);
std::string lumina_type_tostr(lumina_type type);

template<typename... Ts>
struct literal_value_overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
literal_value_overload(Ts...) -> literal_value_overload<Ts...>;

extern std::string literal_tostr(const literal_value& l);

NAMESPACE_END

#endif

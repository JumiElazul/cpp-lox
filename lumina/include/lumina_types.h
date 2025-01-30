#ifndef JUMI_LUMINA_LUMINA_TYPES_H
#define JUMI_LUMINA_LUMINA_TYPES_H
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(lumina)

using literal_value = std::variant<double, bool, std::string, std::monostate>;

template<typename... Ts>
struct literal_value_overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
literal_value_overload(Ts...) -> literal_value_overload<Ts...>;

extern std::string literal_tostr(const literal_value& l);

NAMESPACE_END

#endif

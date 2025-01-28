#ifndef JUMI_LUMINA_TOKENS_H
#define JUMI_LUMINA_TOKENS_H
#include "typedefs.h"
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <variant>

NAMESPACE_BEGIN(lumina)

struct token;

enum class token_type
{
    // single-character tokens
    left_paren_, right_paren_, left_brace_, right_brace_,
    comma_, dot_, semicolon_,

    // one or two character tokens
    minus_, minus_equal_,
    plus_, plus_equal_,
    slash_, slash_equal_,
    star_, star_equal_,
    bang_, bang_equal_,
    equal_, equal_equal_,
    greater_, greater_equal_,
    less_, less_equal_,

    // literals
    identifier_, string_, number_,

    // keywords
    and_, or_, if_, else_, class__, false_, true_, func_, null_,
    print_, return_, super_, this_, var_, for_, while_,

    // end of file/other
    bof_,
    eof_,
    invalid_,
    newline_,
};

using coord = std::pair<uint32, uint32>;
using literal_value = std::variant<double, std::string, std::monostate>;

template<typename... Ts>
struct literal_value_overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
literal_value_overload(Ts...) -> literal_value_overload<Ts...>;

extern std::string get_literal(const token& t);

struct token
{
    token_type type;
    std::string lexeme;
    literal_value literal;
    coord position;
};

extern const std::unordered_map<token_type, std::string> token_type_tostr;
extern std::string to_string(const token& t);

NAMESPACE_END

std::ostream& operator<<(std::ostream& os, const lumina::token& t);

#endif

#ifndef JUMI_LUMINA_TOKENS_H
#define JUMI_LUMINA_TOKENS_H
#include "typedefs.h"
#include "lumina_types.h"
#include <iosfwd>
#include <string>
#include <string_view>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

struct token;

enum class token_type
{
    // single-character tokens
    left_paren_, right_paren_, left_brace_, right_brace_,
    comma_, dot_, semicolon_, colon_, question_, modulo_,

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
    and_, or_, if_, else_, class_, false_, true_, func_, null_,
    print_, return_, super_, this_, var_, for_, while_, break_, continue_,

    // end of file/other
    bof_,
    eof_,
    ignore_,
    invalid_,
};

using coord = std::pair<uint32, uint32>;

struct token
{
    token_type type;
    std::string lexeme;
    literal_value literal;
    coord position;
    std::string_view source_line;
};

extern const std::unordered_map<token_type, std::string> token_type_tostr;
extern std::string debug_to_string(const token& t);

NAMESPACE_END

std::ostream& operator<<(std::ostream& os, const lumina::token& t);

#endif

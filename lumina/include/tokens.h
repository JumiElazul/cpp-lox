#ifndef JUMI_LUMINA_TOKENS_H
#define JUMI_LUMINA_TOKENS_H
#include "typedefs.h"
#include <string>
#include <variant>

NAMESPACE_BEGIN(lumina)
using literal_value = std::variant<double, std::string, std::monostate>;

enum class token_type
{
    // single-character tokens
    left_paren_, right_paren_, left_brace_, right_brace_,
    comma_, dot_, minus_, plus_, semicolon_, slash_, star_,

    // one or two character tokens
    bang_, bang_equal_,
    equal_, equal_equal_,
    greater_, greater_equal_,
    less_, less_equal_,

    // literals
    identifier_, string_, number_,

    // keywords
    and_, or_, if_, else_, class__, false_, true_, fun_, nil_,
    print_, return_, super_, this_, var_, for_, while_,

    // end of file
    bof_,
    eof_
};

struct token
{
    token_type type;
    std::pair<uint32, uint32> position;
    std::string lexeme;
    literal_value literal;
};

NAMESPACE_END

#endif

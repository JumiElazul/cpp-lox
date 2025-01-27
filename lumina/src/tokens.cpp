#include "tokens.h"
#include "typedefs.h"
#include <ostream>
#include <sstream>

NAMESPACE_BEGIN(lumina)

extern const std::unordered_map<token_type, std::string> token_type_tostr =
{
    { token_type::left_paren_,    "("                 },
    { token_type::right_paren_,   ")"                 },
    { token_type::left_brace_,    "{"                 },
    { token_type::right_brace_,   "}"                 },
    { token_type::comma_,         ","                 },
    { token_type::dot_,           "."                 },
    { token_type::semicolon_,     ";"                 },
    { token_type::minus_,         "-"                 },
    { token_type::minus_equal_,   "-="                },
    { token_type::plus_,          "+"                 },
    { token_type::plus_equal_,    "+="                },
    { token_type::slash_,         "/"                 },
    { token_type::slash_equal_,   "/="                },
    { token_type::star_,          "*"                 },
    { token_type::star_equal_,    "*="                },
    { token_type::bang_,          "!"                 },
    { token_type::bang_equal_,    "!="                },
    { token_type::equal_,         "="                 },
    { token_type::equal_equal_,   "=="                },
    { token_type::greater_,       ">"                 },
    { token_type::greater_equal_, ">="                },
    { token_type::less_,          "<"                 },
    { token_type::less_equal_,    "<="                },
    { token_type::identifier_,    "identifier"        },
    { token_type::string_,        "string"            },
    { token_type::number_,        "number"            },
    { token_type::and_,           "and"               },
    { token_type::or_,            "or"                },
    { token_type::if_,            "if"                },
    { token_type::else_,          "else"              },
    { token_type::class__,        "class"             },
    { token_type::false_,         "false"             },
    { token_type::true_,          "true"              },
    { token_type::func_,          "func"              },
    { token_type::nil_,           "nil"               },
    { token_type::print_,         "print"             },
    { token_type::return_,        "return"            },
    { token_type::super_,         "super"             },
    { token_type::this_,          "this"              },
    { token_type::var_,           "var"               },
    { token_type::for_,           "for"               },
    { token_type::while_,         "while"             },
    { token_type::bof_,           "beginning of file" },
    { token_type::eof_,           "end of file"       },
    { token_type::invalid_,       "invalid"           },
    { token_type::newline_,       "invalid"           },
};

std::string to_string(const token& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

NAMESPACE_END

std::ostream& operator<<(std::ostream& os, const lumina::token& t)
{
    os << "token [type: " << lumina::token_type_tostr.at(t.type) << ", lexeme: " << t.lexeme
        << ", line/col: " << t.position.first << ":" << t.position.second << "]";
    return os;
}


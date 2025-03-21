#include "tokens.h"
#include "typedefs.h"
#include <ostream>
#include <sstream>

NAMESPACE_BEGIN(cpplox)

const std::unordered_map<token_type, std::string> token_type_tostr =
{
    { token_type::left_paren_,    "left_paren"        },
    { token_type::right_paren_,   "right_paren"       },
    { token_type::left_brace_,    "left_brace"        },
    { token_type::right_brace_,   "right_brace"       },
    { token_type::comma_,         "comma"             },
    { token_type::dot_,           "dot"               },
    { token_type::semicolon_,     "semicolon"         },
    { token_type::colon_,         "colon"             },
    { token_type::question_,      "question"          },
    { token_type::modulo_,        "modulo"            },
    { token_type::minus_,         "minus"             },
    { token_type::minus_equal_,   "minus_equal"       },
    { token_type::minus_minus_,   "minus_minus"       },
    { token_type::plus_,          "plus"              },
    { token_type::plus_equal_,    "plus_equal"        },
    { token_type::plus_plus_,     "plus_plus"         },
    { token_type::slash_,         "slash"             },
    { token_type::slash_equal_,   "slash_equal"       },
    { token_type::star_,          "star"              },
    { token_type::star_equal_,    "star_equal"        },
    { token_type::bang_,          "bang"              },
    { token_type::bang_equal_,    "bang_equal"        },
    { token_type::equal_,         "equal"             },
    { token_type::equal_equal_,   "equal_equal"       },
    { token_type::greater_,       "greater"           },
    { token_type::greater_equal_, "greater_equal"     },
    { token_type::less_,          "less"              },
    { token_type::less_equal_,    "less_equal"        },
    { token_type::identifier_,    "identifier"        },
    { token_type::string_,        "string"            },
    { token_type::number_,        "number"            },
    { token_type::and_,           "and"               },
    { token_type::or_,            "or"                },
    { token_type::if_,            "if"                },
    { token_type::else_,          "else"              },
    { token_type::class_,         "class"             },
    { token_type::false_,         "false"             },
    { token_type::true_,          "true"              },
    { token_type::func_,          "func"              },
    { token_type::null_,          "null"              },
    { token_type::return_,        "return"            },
    { token_type::super_,         "super"             },
    { token_type::this_,          "this"              },
    { token_type::var_,           "var"               },
    { token_type::for_,           "for"               },
    { token_type::while_,         "while"             },
    { token_type::break_,         "break"             },
    { token_type::continue_,      "continue"          },
    { token_type::continue_,      "static"            },
    { token_type::bof_,           "bof"               },
    { token_type::eof_,           "eof"               },
    { token_type::ignore_,        "ignore"            },
    { token_type::invalid_,       "invalid"           },

    { token_type::debug_,         "debug"             },
};

std::string debug_to_string(const token& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

token create_dummy_token(token_type type)
{
    token t{ type, token_type_tostr.at(type), "", { 0, 0 }, "" };
    return t;
}

NAMESPACE_END

std::ostream& operator<<(std::ostream& os, const cpplox::token& t)
{
    os << "token [type: "
       << cpplox::token_type_tostr.at(t.type)
       << ", lexeme: " << t.lexeme
       << ", literal: " << cpplox::literal_value_to_runtime_string(t.literal)
       << ", line/col: " << t.position.first << ":" << t.position.second
       << "]";
    return os;
}


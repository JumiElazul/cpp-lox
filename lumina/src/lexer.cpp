#include "lexer.h"
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <cctype>
#include <optional>
#include <vector>
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

lexer::lexer(console_io* io)
    : _lexer_state()
    , _character_map()
    , _reserved_keyword_map()
    , _io(io)
{
    _character_map =
    {
        { '(',  &lexer::left_paren     },
        { ')',  &lexer::right_paren    },
        { '{',  &lexer::left_brace     },
        { '}',  &lexer::right_brace    },
        { ',',  &lexer::comma          },
        { '.',  &lexer::dot            },
        { '-',  &lexer::minus          },
        { '+',  &lexer::plus           },
        { ';',  &lexer::semicolon      },
        { ':',  &lexer::colon          },
        { '?',  &lexer::question       },
        { '/',  &lexer::slash          },
        { '*',  &lexer::star           },
        { '!',  &lexer::bang           },
        { '=',  &lexer::equal          },
        { '>',  &lexer::greater        },
        { '<',  &lexer::less           },
        { '"',  &lexer::string         },
    };

    _reserved_keyword_map = 
    {
        { "and",    token_type::and_    },
        { "or",     token_type::or_     },
        { "if",     token_type::if_     },
        { "else",   token_type::else_   },
        { "class",  token_type::class_  },
        { "false",  token_type::false_  },
        { "true",   token_type::true_   },
        { "func",   token_type::func_   },
        { "null",   token_type::null_   },
        { "print",  token_type::print_  },
        { "return", token_type::return_ },
        { "super",  token_type::super_  },
        { "this",   token_type::this_   },
        { "var",    token_type::var_    },
        { "for",    token_type::for_    },
        { "while",  token_type::while_  },
    };
}

std::vector<token> lexer::tokenize(const std::string& input)
{
    _lexer_state = lexer_state{};
    _lexer_state.input = std::move(input);

    std::vector<token> tokens;
    tokens.reserve(input.size() / 3);
    tokens.push_back(create_token(token_type::bof_));

    size_t input_size = _lexer_state.input.size();
    while (_lexer_state.right_ptr < input_size)
    {
        token next_token = fetch_token();
        _lexer_state.current_pos += static_cast<uint32>(next_token.lexeme.size());

        if (next_token.type != token_type::invalid_)
        {
            tokens.push_back(next_token);
        }
    }

    return tokens;
}

token lexer::fetch_token()
{
    _lexer_state.left_ptr = _lexer_state.right_ptr;
    std::optional<char> current_char = advance_lexer();

    if (current_char.has_value())
    {
        if (isalpha(*current_char) || *current_char == '_')
        {
            return identifier();
        }
        else if (isdigit(*current_char))
        {
            return number();
        }

        auto char_map_find = _character_map.find(*current_char);
        if (char_map_find != _character_map.end())
        {
            return (this->*char_map_find->second)();
        }
    }
    return create_token(token_type::invalid_);
}

std::optional<char> lexer::advance_lexer()
{
    if (_lexer_state.right_ptr >= _lexer_state.input.size())
        return std::nullopt;

    char c = _lexer_state.input[_lexer_state.right_ptr];
    ++_lexer_state.right_ptr;

    if (c == '\n')
    {
        ++_lexer_state.current_line;

        // We need to set the current_pos as one less than the default value of 1, because the tokenize method will
        // increment the current_pos by the length of the token (in this case 1, putting the current_pos at the proper value)
        _lexer_state.current_pos = 0;
    }

    return c;
}

std::optional<char> lexer::peek_next()
{
    if (_lexer_state.right_ptr >= _lexer_state.input.size())
        return std::nullopt;

    return _lexer_state.input[_lexer_state.right_ptr];
}

bool lexer::advance_if_next_matches(char c)
{
    if (peek_next() == c)
    {
        advance_lexer();
        return true;
    }

    return false;
}

token lexer::create_token(token_type type, const literal_value& literal)
{
    uint32 len = extract_lexeme_length();
    std::string lexeme = _lexer_state.input.substr(_lexer_state.left_ptr, len);
    return token{ type, lexeme, literal, { _lexer_state.current_line, _lexer_state.current_pos } };
}

uint32 lexer::extract_lexeme_length() const noexcept
{
    return _lexer_state.right_ptr - _lexer_state.left_ptr;
}

token lexer::left_paren()
{
    return create_token(token_type::left_paren_);
}

token lexer::right_paren()
{
    return create_token(token_type::right_paren_);
}

token lexer::left_brace()
{
    return create_token(token_type::left_brace_);
}

token lexer::right_brace()
{
    return create_token(token_type::right_brace_);
}

token lexer::comma()
{
    return create_token(token_type::comma_);
}

token lexer::dot()
{
    return create_token(token_type::dot_);
}

token lexer::minus()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::minus_equal_);

    return create_token(token_type::minus_);
}

token lexer::plus()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::plus_equal_);

    return create_token(token_type::plus_);
}

token lexer::semicolon()
{
    return create_token(token_type::semicolon_);
}

token lexer::colon()
{
    return create_token(token_type::colon_);
}

token lexer::question()
{
    return create_token(token_type::question_);
}

token lexer::slash()
{
    if (!peek_next().has_value()) 
    {
        return create_token(token_type::slash_);
    }

    char c = *peek_next();

    switch (c)
    {
        case '=':
        {
            advance_lexer();
            return create_token(token_type::slash_equal_);
        }
        case '/':
        {
            advance_lexer();

            while (peek_next().has_value() && *peek_next() != '\n')
                advance_lexer();

            return create_token(token_type::invalid_);
        }
        case '*':
        {
            advance_lexer();

            while (true)
            {
                // Hit end of input
                if (!peek_next().has_value())
                    break;

                // End of block comment
                if (*peek_next() == '*')
                {
                    advance_lexer();

                    if (peek_next().has_value() && *peek_next() == '/')
                    {
                        advance_lexer();
                        break;
                    }
                }
                else
                {
                    advance_lexer();
                }
            }

            return create_token(token_type::invalid_);
        }
    }

    return create_token(token_type::slash_);
}

token lexer::star()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::star_equal_);

    return create_token(token_type::star_);
}

token lexer::bang()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::bang_equal_);

    return create_token(token_type::bang_);
}

token lexer::equal()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::equal_equal_);

    return create_token(token_type::equal_);
}

token lexer::greater()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::greater_equal_);

    return create_token(token_type::greater_);
}

token lexer::less()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::less_equal_);

    return create_token(token_type::less_);
}

token lexer::identifier()
{
    while (peek_next().has_value() && (isalnum(*peek_next()) || *peek_next() == '_'))
    {
        advance_lexer();
    }

    std::string substr = _lexer_state.input.substr(_lexer_state.left_ptr, extract_lexeme_length());
    auto reserved_keyword = _reserved_keyword_map.find(substr);
    if (reserved_keyword != _reserved_keyword_map.end())
    {
        return create_token(reserved_keyword->second);
    }

    return create_token(token_type::identifier_);
}

token lexer::string()
{
    while (peek_next().has_value() && peek_next() != '"')
    {
        advance_lexer();
    }

    if (peek_next().has_value() && peek_next() == '"')
    {
        advance_lexer();
        uint32 literal_len = extract_lexeme_length() - 2;
        std::string str = _lexer_state.input.substr(_lexer_state.left_ptr + 1, literal_len);

        return create_token(token_type::string_, str);
    }

    // Unterminated string
    return invalid_token();
}

token lexer::number()
{
    while (peek_next().has_value() && isdigit(*peek_next()))
    {
        advance_lexer();
    }

    if (peek_next() == '.')
    {
        advance_lexer();
        while (peek_next().has_value() && isdigit(*peek_next()))
        {
            advance_lexer();
        }
    }

    uint32 len = extract_lexeme_length();
    double d = std::stod(_lexer_state.input.substr(_lexer_state.left_ptr, len));
    return create_token(token_type::number_, d);
}

token lexer::invalid_token()
{
    return create_token(token_type::invalid_);
}

NAMESPACE_END


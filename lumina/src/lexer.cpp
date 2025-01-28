#include "lexer.h"
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <optional>
#include <vector>
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

lexer::lexer(console_io* io)
    : _lexer_state()
    , _character_map()
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
        { '/',  &lexer::slash          },
        { '*',  &lexer::star           },
        { '!',  &lexer::bang           },
        { '=',  &lexer::equal          },
        { '>',  &lexer::greater        },
        { '<',  &lexer::less           },
        { '\n', &lexer::newline        },
        { ' ',  &lexer::whitespace     },
        { '\r', &lexer::whitespace     },
        { '\t', &lexer::whitespace     },
        { '"',  &lexer::string         },
        { '1',  &lexer::number         },
        { '2',  &lexer::number         },
        { '3',  &lexer::number         },
        { '4',  &lexer::number         },
        { '5',  &lexer::number         },
        { '6',  &lexer::number         },
        { '7',  &lexer::number         },
        { '8',  &lexer::number         },
        { '9',  &lexer::number         },
        { '0',  &lexer::number         },
    };
}

std::vector<token> lexer::tokenize(const std::string& input)
{
    _lexer_state = lexer_state{};
    _lexer_state.input = std::move(input);

    std::vector<token> tokens;
    tokens.reserve(input.size() / 3);
    tokens.push_back(create_token(token_type::bof_, std::monostate{}));

    while (_lexer_state.right_ptr < _lexer_state.input.size())
    {
        token next_token = fetch_token();

        switch (next_token.type)
        {
            case token_type::newline_:
            {
                _lexer_state.current_line += 1;
                _lexer_state.current_pos = 0;
            } break;
            case token_type::invalid_:
            {
                ++_lexer_state.current_pos;
            } break;
            default:
            {
                size_t lexeme_len = next_token.lexeme.size();
                _lexer_state.current_pos += static_cast<uint32>(lexeme_len);
                tokens.push_back(next_token);
            } break;
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
        auto char_map_find = _character_map.find(*current_char);
        if (char_map_find != _character_map.end())
        {
            return (this->*char_map_find->second)();
        }
    }
    return create_token(token_type::invalid_, std::monostate{});
}

std::optional<char> lexer::advance_lexer()
{
    if (_lexer_state.right_ptr >= _lexer_state.input.size())
        return std::nullopt;

    return _lexer_state.input[_lexer_state.right_ptr++];
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

token lexer::create_token(token_type type, const literal_value& literal, bool string)
{
    if (string)
    {
        uint32 len = _lexer_state.right_ptr - _lexer_state.left_ptr - 2;
        std::string lexeme = _lexer_state.input.substr(_lexer_state.left_ptr + 1, len);
        return token(type, lexeme, literal, { _lexer_state.current_line, _lexer_state.current_pos });
    }
    else
    {
        uint32 len = _lexer_state.right_ptr - _lexer_state.left_ptr;
        std::string lexeme = _lexer_state.input.substr(_lexer_state.left_ptr, len);
        return token(type, lexeme, literal, { _lexer_state.current_line, _lexer_state.current_pos });
    }
}

token lexer::left_paren()
{
    return create_token(token_type::left_paren_,  std::monostate{});
}

token lexer::right_paren()
{
    return create_token(token_type::right_paren_, std::monostate{});
}

token lexer::left_brace()
{
    return create_token(token_type::left_brace_, std::monostate{});
}

token lexer::right_brace()
{
    return create_token(token_type::right_brace_, std::monostate{});
}

token lexer::comma()
{
    return create_token(token_type::comma_, std::monostate{});
}

token lexer::dot()
{
    return create_token(token_type::dot_, std::monostate{});
}

token lexer::minus()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::minus_equal_, std::monostate{});

    return create_token(token_type::minus_, std::monostate{});
}

token lexer::plus()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::plus_equal_, std::monostate{});

    return create_token(token_type::plus_, std::monostate{});
}

token lexer::semicolon()
{
    return create_token(token_type::semicolon_, std::monostate{});
}

token lexer::slash()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::slash_equal_, std::monostate{});

    return create_token(token_type::slash_, std::monostate{});
}

token lexer::star()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::star_equal_, std::monostate{});

    return create_token(token_type::star_, std::monostate{});
}

token lexer::bang()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::bang_equal_, std::monostate{});

    return create_token(token_type::bang_, std::monostate{});
}

token lexer::equal()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::equal_equal_, std::monostate{});

    return create_token(token_type::equal_, std::monostate{});
}

token lexer::greater()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::greater_equal_, std::monostate{});

    return create_token(token_type::greater_, std::monostate{});
}

token lexer::less()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::less_equal_, std::monostate{});

    return create_token(token_type::less_, std::monostate{});
}

token lexer::newline()
{
    return create_token(token_type::newline_, std::monostate{});
}

token lexer::whitespace()
{
    return invalid_token();
}

token lexer::string()
{
    while (peek_next() != '"' && peek_next().has_value())
    {
        advance_lexer();
    }

    if (peek_next() == '"')
    {
        advance_lexer();
        return create_token(token_type::string_, std::monostate{}, true);
    }

    // Untermintated string
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

    return create_token(token_type::number_, std::monostate{});
}

token lexer::invalid_token()
{
    return create_token(token_type::invalid_, std::monostate{});
}

NAMESPACE_END


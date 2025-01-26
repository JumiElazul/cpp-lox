#include "lexer.h"
#include "lexer_exceptions.h"
#include "typedefs.h"
#include "tokens.h"
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

lexer::lexer()
    : _lexer_state()
    , _character_map()
{
    _character_map =
    {
        { '(', &lexer::left_paren    },
        { ')', &lexer::right_paren   },
    };
}

std::vector<token> lexer::tokenize(const std::string& input)
{
    reset_lexer_state();
    _lexer_state.input_length = input.size();
    _lexer_state.input = input;

    std::vector<token> tokens;
    tokens.emplace_back(create_token(token_type::bof_, { 0, 0 }, "", std::monostate{}));

    if (_lexer_state.input_length != 0)
    {
        token next_token = fetch_token();
    }

    tokens.emplace_back(create_token(token_type::eof_, { _lexer_state.right_index, _lexer_state.right_index }, "", std::monostate{}));

    return tokens;
}

token lexer::fetch_token()
{
    char current_char = advance_lexer();


}

char lexer::advance_lexer()
{
    if (_lexer_state.right_index >= _lexer_state.input_length)
        throw lexer_exception("lexer::advance_lexer(), lexer reached end of input");

    return _lexer_state.input[_lexer_state.right_index++];
}

char lexer::peek_next()
{
    if (_lexer_state.right_index >= _lexer_state.input_length)
        throw lexer_exception("lexer::peek_next(), lexer reached end of input");

    return _lexer_state.input[_lexer_state.right_index];
}

token create_token(token_type type, const std::pair<uint32, uint32>& position, const std::string& lexeme, const literal_value& literal)
{
    return token(type, position, lexeme, literal);
}

void lexer::reset_lexer_state() noexcept
{
    _lexer_state.current_line = 1;
    _lexer_state.left_index = 0;
    _lexer_state.right_index = 0;
    _lexer_state.input_length = 0;
}

void lexer::left_paren()
{

}

void lexer::right_paren()
{

}

NAMESPACE_END


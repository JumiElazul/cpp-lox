#include "lexer.h"
#include "typedefs.h"
#include "tokens.h"
#include <vector>

NAMESPACE_BEGIN(lumina)

lexer::lexer()
    : _lexer_state()
{
}

std::vector<token> lexer::tokenize(const std::string& input)
{
    reset_lexer_state();
    std::vector<token> tokens;
    tokens.emplace_back(create_token(token_type::bof_, { 0, 0 }, "", std::monostate{}));

    token next_token = fetch_token();

    tokens.emplace_back(create_token(token_type::eof_, { _lexer_state.end_index, _lexer_state.end_index }, "", std::monostate{}));
    return tokens;
}

token lexer::fetch_token()
{

}

char lexer::advance_lexer()
{

}

char lexer::peek_next()
{

}

token create_token(token_type type, const std::pair<uint32, uint32>& position, const std::string& lexeme, const literal_value& literal)
{
    return token(type, position, lexeme, literal);
}

void lexer::reset_lexer_state() noexcept
{
    _lexer_state.current_line = 1;
    _lexer_state.start_index = 0;
    _lexer_state.end_index = 0;
}

NAMESPACE_END


#ifndef JUMI_LUMINA_LEXER_H
#define JUMI_LUMINA_LEXER_H
#include "typedefs.h"
#include "tokens.h"
#include <string>
#include <vector>

NAMESPACE_BEGIN(lumina)

class lexer
{
struct lexer_state
{
    uint32 current_line;
    uint32 start_index;
    uint32 end_index;
};

public:
    lexer();
    std::vector<token> tokenize(const std::string& input);

private:
    lexer_state _lexer_state;

    token fetch_token();
    char advance_lexer();
    char peek_next();
    token create_token(token_type type, const std::pair<uint32, uint32>& position, const std::string& lexeme, const literal_value& literal);
    void reset_lexer_state() noexcept;
};

NAMESPACE_END

#endif

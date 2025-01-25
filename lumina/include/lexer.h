#ifndef JUMI_LUMINA_LEXER_H
#define JUMI_LUMINA_LEXER_H
#include "typedefs.h"
#include "tokens.h"
#include <string>
#include <vector>

NAMESPACE_BEGIN(lumina)

class lexer
{
public:
    lexer();
    std::vector<token> tokenize(const std::string& input);

private:
};

NAMESPACE_END

#endif

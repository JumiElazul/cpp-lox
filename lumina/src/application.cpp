#include "application.h"
#include "typedefs.h"
#include "console_io.h"
#include "logger.h"
#include "lexer.h"
#include <vector>

NAMESPACE_BEGIN(lumina)

application::application()
    : _io(std::make_unique<console_io>())
    , _lexer()
{

}

application::~application() = default;

void application::run()
{
    LUMINA_PRINT_LOG_LEVELS;
    LUMINA_INFO("application::run() started");

    while (true)
    {
        _io->write("lumina > ");
        std::string input = _io->readline();
        
        if (input == "q" || input == "quit")
            break;

        std::vector<token> tokens = _lexer.tokenize(input);
    }
}

NAMESPACE_END

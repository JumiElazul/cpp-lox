#include "lumina_app.h"
#include "typedefs.h"
#include "console_io.h"
#include "logger.h"
#include "lexer.h"
#include <vector>

NAMESPACE_BEGIN(lumina)

lumina_app::lumina_app()
    : _io(std::make_unique<console_io>())
    , _lexer(_io.get())
{

}

lumina_app::~lumina_app() = default;

void lumina_app::run()
{
    LUMINA_PRINT_LOG_LEVELS;
    LUMINA_INFO("lumina_app::run() started");

    while (true)
    {
        _io->write("lumina > ");
        std::string input = _io->readline();
        
        if (input == "q" || input == "quit")
            break;

        std::vector<token> tokens = _lexer.tokenize(input);

        for (const token& t : tokens)
        {
            *_io << t << '\n';
            // LUMINA_INFO(to_string(t));
        }
    }
}

NAMESPACE_END

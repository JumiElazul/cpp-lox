#include "application.h"
#include "typedefs.h"
#include "console_io.h"
#include "logger.h"

NAMESPACE_BEGIN(lumina)

application::application()
    : _io(std::make_unique<console_io>())
{

}

application::~application() = default;

void application::run()
{
    LUMINA_PRINT_LOG_LEVELS;

    while (true)
    {
        _io->write("lumina > ");
        std::string input = _io->readline();
        
        if (input == "q" || input == "quit")
            break;

        _io->write({ "input was ", input , "\n"});
        LUMINA_TRACE("input was " + input);
    }
}

NAMESPACE_END

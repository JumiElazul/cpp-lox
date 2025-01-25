#include "application.h"
#include "typedefs.h"
#include "console_io.h"
#include <memory>
#include <string>

NAMESPACE_BEGIN(lumina)

application::application()
    : _io(std::make_unique<console_io>())
{

}

application::~application() = default;

void application::run()
{
    while (true)
    {
        _io->write("lumina > ");
        std::string input = _io->readline();
        
        if (input == "q" || input == "quit")
            break;

        _io->write({ "input was ", input , "\n"});
    }
}

NAMESPACE_END

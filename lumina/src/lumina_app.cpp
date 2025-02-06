#include "lumina_app.h"
#include "console_io.h"
#include "expression_visitors.h"
#include "interpreter.h"
#include "logger.h"
#include "lexer.h"
#include "parser.h"
#include "typedefs.h"
#include "statements.h"
#include <vector>
#include <fstream>
#include <memory>
#include <sstream>

NAMESPACE_BEGIN(lumina)

lumina_app::lumina_app()
    : _io(std::make_unique<console_io>())
    , _lexer()
    , _env()
    , _had_runtime_error(false)
{

}

lumina_app::~lumina_app() = default;

void lumina_app::run_file_mode(const char* filepath)
{
    LUMINA_PRINT_LOG_LEVELS;
    LUMINA_INFO("lumina_app::run_file_mode() started");

    _io->out() << "Lumina v" << LUMINA_VERSION << " (type 'q' or 'quit' to exit)\n";

    std::ifstream file(filepath);
    if (!file)
    {
        _io->err() << "File with path [" << filepath << "could not be read\n";
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string contents = ss.str();

    std::vector<token> tokens = _lexer.tokenize(contents);

    std::unique_ptr<parser> parser = std::make_unique<recursive_descent_parser>(tokens, _io.get());
    std::vector<std::unique_ptr<statement>> statements = parser->parse();

    if (parser->error_occurred())
    {
        _had_runtime_error = true;
        return;
    }

    interpreter i(_env, _io.get());
    i.interpret(statements);
}

void lumina_app::run_interpreter_mode()
{
    LUMINA_PRINT_LOG_LEVELS;
    LUMINA_INFO("lumina_app::run_interpreter_mode() started");

    _io->out() << "Lumina v" << LUMINA_VERSION << " (type 'q' or 'quit' to exit)\n";

    while (true)
    {
        _had_runtime_error = false;

        _io->out() << "lumina >> ";
        std::string input = _io->readline();
        
        if (input == "q" || input == "quit")
            break;

        std::vector<token> tokens = _lexer.tokenize(input);

        std::unique_ptr<parser> parser = std::make_unique<recursive_descent_parser>(tokens, _io.get());
        std::vector<std::unique_ptr<statement>> statements = parser->parse();

        if (parser->error_occurred())
        {
            _had_runtime_error = true;
            continue;
        }

        interpreter i(_env, _io.get());
        i.interpret(statements);
    }
}

NAMESPACE_END

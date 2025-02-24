#include "geo_app.h"
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

NAMESPACE_BEGIN(geo)

geo_app::geo_app()
    : _io(std::make_unique<console_io>())
    , _interpreter(_io.get())
    , _resolver(_interpreter)
    , _had_runtime_error(false) { }

geo_app::~geo_app() = default;

void geo_app::run_file_mode(const char* filepath)
{
    GEO_PRINT_LOG_LEVELS;
    GEO_INFO("geo_app::run_file_mode() started");

    std::ifstream file(filepath);
    if (!file)
    {
        _io->err() << "File with path [" << filepath << "] could not be read\n";
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();
    run(source);
}

void geo_app::run_interpreter_mode()
{
    GEO_PRINT_LOG_LEVELS;
    GEO_INFO("geo_app::run_interpreter_mode() started");

    _io->out() << "geo v" << GEO_VERSION << " (type 'q' or 'quit' to exit)\n";

    while (true)
    {
        _had_runtime_error = false;

        std::string source = _io->readline("geo >> ");
        
        if (source == "q" || source == "quit")
            break;

        run(source);
    }
}

void geo_app::run(const std::string& source)
{
    // 1. Lexing Phase
    lexer l(source, _io.get());

    if (l.error_occurred())
        return;

    const std::vector<token>& tokens = l.get_tokens();

    // 2. Parsing Phase
    recursive_descent_parser parser(tokens, _io.get());

    // NOTE(Greg): Currently, this vector owns the statements that are parsed. This will become a problem
    // once we introduce multiple file projects, as well as some cases in REPL mode where these
    // statements will go out of scope once the user executes a line.
    std::vector<std::unique_ptr<statement>> statements = parser.parse();

    if (parser.error_occurred())
    {
        _had_runtime_error = true;
        return;
    }

    // 3. Static Analysis
    _resolver.resolve_all(statements);

    // 4. Interpreter
    _interpreter.interpret(statements);
}

NAMESPACE_END

#include "cpplox_app.h"
#include "console_io.h"
#include "debug_timer.h"
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

NAMESPACE_BEGIN(cpplox)

cpplox_app::cpplox_app()
    : _io(std::make_unique<console_io>())
    , _interpreter(_io.get())
    , _resolver(_interpreter)
    , _statements()
    , _had_runtime_error(false) 
{
    _statements.reserve(128); 
    CPPLOX_INFO("--------------------------------------------------");
    CPPLOX_INFO("Geo version " CPPLOX_VERSION " started running");
    CPPLOX_INFO("--------------------------------------------------");
}

cpplox_app::~cpplox_app()
{
    CPPLOX_INFO("--------------------------------------------------");
    CPPLOX_INFO("Geo version " CPPLOX_VERSION " finished running");
    CPPLOX_INFO("--------------------------------------------------");
}

void cpplox_app::run_file_mode(const char* filepath)
{
    CPPLOX_PRINT_LOG_LEVELS;

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

void cpplox_app::run_interpreter_mode()
{
    CPPLOX_PRINT_LOG_LEVELS;
    CPPLOX_INFO("cpplox_app::run_interpreter_mode() started");

    _io->out() << "cpplox v" << CPPLOX_VERSION << " (type 'q' or 'quit' to exit)\n";

    while (true)
    {
        _had_runtime_error = false;

        std::string source = _io->readline("cpp-lox >> ");
        
        if (source == "q" || source == "quit")
            break;

        run(source);
    }
}

void cpplox_app::run(const std::string& source)
{
    // 1. Lexing Phase
    lexer l(source, _io.get());

    if (l.error_occurred())
        return;

    const std::vector<token>& tokens = l.get_tokens();

    // 2. Parsing Phase
    recursive_descent_parser parser(tokens, _io.get());

    std::vector<std::unique_ptr<statement>> statements = parser.parse();

    if (parser.error_occurred())
    {
        _had_runtime_error = true;
        return;
    }

    // 3. Static Analysis
    _resolver.resolve_all(statements);

    if (_resolver.error_occurred())
    {
        _had_runtime_error = true;
        return;
    }

    // 4. Interpreter
    _interpreter.interpret(statements);

    store_statements(std::move(statements));

    CPPLOX_TRACE("--------------------------------------------------");
    CPPLOX_TRACE("[ Execution Time ]");
    for (const auto& time : execution_times_us)
        CPPLOX_TRACE(format_execution_time(time.first, time.second));
    CPPLOX_TRACE("--------------------------------------------------");
}

void cpplox_app::store_statements(std::vector<std::unique_ptr<statement>>&& statements)
{
    for (size_t i = 0; i < statements.size(); ++i)
    {
        _statements.push_back(std::move(statements[i]));
    }
}

NAMESPACE_END

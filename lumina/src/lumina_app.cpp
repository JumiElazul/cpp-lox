#include "lumina_app.h"
#include "console_io.h"
#include "expressions.h"
#include "expression_visitors.h"
#include "logger.h"
#include "lexer.h"
#include "parser.h"
#include "typedefs.h"
#include <vector>
#include <fstream>
#include <memory>
#include <sstream>

NAMESPACE_BEGIN(lumina)

lumina_app::lumina_app()
    : _io(std::make_unique<console_io>())
    , _lexer(_io.get())
{

}

lumina_app::~lumina_app() = default;

void lumina_app::run_file_mode(const char* filepath)
{
    LUMINA_PRINT_LOG_LEVELS;
    LUMINA_INFO("lumina_app::run_file_mode() started");

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
    parser->parse();

    for (const token& t : tokens)
    {
        _io->out() << t << '\n';
    }

    tree_printer printer(_io.get());
}

void lumina_app::run_interpreter_mode()
{
    LUMINA_PRINT_LOG_LEVELS;
    LUMINA_INFO("lumina_app::run_interpreter_mode() started");

    while (true)
    {
        _io->out() << "lumina > ";
        std::string input = _io->readline();
        
        if (input == "q" || input == "quit")
            break;

        std::vector<token> tokens = _lexer.tokenize(input);

        for (const token& t : tokens)
        {
            _io->out() << t << '\n';
        }

        std::unique_ptr<parser> parser = std::make_unique<recursive_descent_parser>(tokens, _io.get());
        std::unique_ptr<expression> parse_tree = parser->parse();

        if (parser->error_occurred())
            continue;

        string_visitor visitor;
        std::string expr_str = parse_tree->accept_visitor(visitor);
        _io->out() << "parser:\n";
        _io->out() << "--------------------\n";
        _io->out() << expr_str << '\n';
    }
}

NAMESPACE_END

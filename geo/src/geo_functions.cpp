#include "geo_functions.h"
#include "console_io.h"
#include "interpreter.h"
#include "statements.h"
#include <cassert>
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <random>

NAMESPACE_BEGIN(geo)

geo_function::geo_function(std::unique_ptr<function_declaration_statement> declaration_)
    : declaration(std::move(declaration_)) { }

int geo_function::arity() { return static_cast<int>(declaration->params.size()); }
std::string geo_function::to_string() const { return "<user_func " + declaration->ident_name.lexeme + ">"; }

literal_value geo_function::call(interpreter& i, const std::vector<literal_value>& args)
{
    std::shared_ptr<environment> function_environment = std::make_shared<environment>(i.current_environment());

    for (size_t i = 0; i < declaration->params.size(); ++i)
        function_environment->define(declaration->params.at(i).lexeme, args.at(i));

    block_statement* body_ptr = dynamic_cast<block_statement*>(declaration->body.get());
    if (!body_ptr)
        throw geo_runtime_error("Function body is not a block statement");

    try
    {
        i.execute_block(body_ptr->statements, function_environment);
    }
    catch (const interpreter::geo_function_return& ret)
    {
        return ret.return_val;
    }
    return std::monostate{};
}

int clock::arity() { return 0; }
std::string clock::to_string() const { return "<native_func>clock"; }

literal_value clock::call(interpreter& i, const std::vector<literal_value>& args)
{
    auto now = std::chrono::system_clock::now();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    return static_cast<double>(ms_since_epoch) / 1000.0;
}

print::print(console_io* io)
    : _io(io) { }

int print::arity() { return 1; }
std::string print::to_string() const { return "<native_func>print"; }

literal_value print::call(interpreter& i, const std::vector<literal_value>& args)
{
    for (const auto& arg : args)
    {
        _io->out() << literal_tostr(arg) << ' ';
    }
    _io->out() << '\n';
    return std::monostate();
}

input::input(console_io* io)
    : _io(io) { }

int input::arity() { return 0; }
std::string input::to_string() const { return "<native_func>input"; }

literal_value input::call(interpreter& i, const std::vector<literal_value>& args)
{
    return _io->readline("");
}

int random::arity() { return 2; }
std::string random::to_string() const { return "<native_func>random"; }

literal_value random::call(interpreter& i, const std::vector<literal_value>& args)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    double min = std::get<double>(args[0]);
    double max = std::get<double>(args[1]);

    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}


NAMESPACE_END

#include "geo_functions.h"
#include "console_io.h"
#include "environment.h"
#include "interpreter.h"
#include <cassert>
#include <chrono>

NAMESPACE_BEGIN(geo)

user_function::user_function(function_declaration_statement& declaration_,
        environment* closure_,
        environment_manager* env_manager)
    : declaration(declaration_)
    , closure(closure_)
    , _env_manager(env_manager) { }

int user_function::arity() { return static_cast<int>(declaration.params.size()); }
std::string user_function::to_string() const { return std::string("<user fn>" + declaration.ident_name.lexeme); }

literal_value user_function::call(interpreter& i, const std::vector<literal_value>& args)
{
    _env_manager->push_environment(closure);
    for (size_t i = 0; i < declaration.params.size(); ++i)
    {
        _env_manager->get_current_environment()->define(declaration.params[i].lexeme, args[i]);
    }

    try
    {
        i.execute_block(declaration.body, _env_manager->get_current_environment());
    }
    catch (const interpreter::geo_function_return& ret)
    {
        _env_manager->pop_environment();
        return ret.return_val;
    }

    _env_manager->pop_environment();
    return std::monostate{};
}

clock::clock() {}
int clock::arity() { return 0; }
std::string clock::to_string() const { return "<native fn>clock"; }

literal_value clock::call(interpreter& i, const std::vector<literal_value>& args)
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = duration_cast<milliseconds>(duration).count();
    return static_cast<double>(millis);
}

print::print(console_io* io) : _io(io) {}
int print::arity() { return 1; }
std::string print::to_string() const { return "<native fn>print"; }

literal_value print::call(interpreter& i, const std::vector<literal_value>& args)
{
    for (const auto& arg : args)
        _io->out() << literal_tostr(arg) << '\n';

    return std::monostate{};
}

NAMESPACE_END

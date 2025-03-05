#include "geo_types.h"
#include "interpreter.h"
#include "typedefs.h"
#include "memory_manager.h"
#include "statements.h"
#include <chrono>
#include <string>
#include <unordered_map>
#include <variant>

NAMESPACE_BEGIN(geo)

std::string geo_type_to_string(geo_type type)
{
    switch (type)
    {
        case geo_type::number_:     return "number";
        case geo_type::string_:     return "string";
        case geo_type::bool_:       return "bool";
        case geo_type::callable_:   return "callable";
        case geo_type::class_:      return "class";
        case geo_type::instance_:   return "instance";
        case geo_type::null_:       return "null";
        case geo_type::undefined_:  return "undefined";
    }

    return "unknown";
}

geo_type literal_to_geo_type(const literal_value& l)
{
    return std::visit(
        literal_value_overload{
            [](double)                      { return geo_type::number_;    },
            [](bool)                        { return geo_type::bool_;      },
            [](const std::string&)          { return geo_type::string_;    },
            [](const geo_callable*)         { return geo_type::callable_;  },
            [](const geo_class*)            { return geo_type::class_;     },
            [](const geo_instance*)         { return geo_type::instance_;  },
            [](std::monostate)              { return geo_type::null_;      },
            [](const undefined&)            { return geo_type::undefined_; },
        }, l);
}

std::string literal_value_to_runtime_string(const literal_value& l)
{
    auto format_number = [](double d) -> std::string {
        std::string s = std::to_string(d);
        size_t i = s.find_last_not_of('0');
        if (s[i] == '.')
            return s.substr(0, i);

        return s.substr(0, i + 1);
    };

    return std::visit(
        literal_value_overload{
            [&](double d)                      { return format_number(d);                               },
            [&](bool b)                        { return b ? std::string("true") : std::string("false"); },
            [&](const std::string& s)          { return s;                                              },
            [&](const geo_callable* c)         { return c->to_string();                                 },
            [&](const geo_class* c)            { return c->name;                                        },
            [&](const geo_instance* i)         { return i->to_string();                                 },
            [&](std::monostate)                { return std::string("null");                            },
            [&](const undefined& u)            { return std::string("undefined");                       },
        }, l);
}

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
        _io->out() << literal_value_to_runtime_string(arg) << '\n';

    return std::monostate{};
}

input::input(console_io* io) : _io(io) {}
int input::arity() { return 0; }
std::string input::to_string() const { return "<native fn>input"; }

literal_value input::call(interpreter& i, const std::vector<literal_value>& args)
{
    std::string value = _io->readline("");
    return value;
}

geo_class::geo_class(const std::string& name_)
    : name(name_) { }

int geo_class::arity() { return 0; }
std::string geo_class::to_string() const { return "<class>" + name; }

literal_value geo_class::call(interpreter& i, const std::vector<literal_value>& args)
{
    return memory_manager::instance().allocate_instance(this);
}

geo_instance::geo_instance(geo_class* class_)
    : _class(class_) { }

std::string geo_instance::to_string() const
{
    return _class->name + " instance";
}

NAMESPACE_END

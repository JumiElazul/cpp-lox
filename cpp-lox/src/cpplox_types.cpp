#include "cpplox_types.h"
#include "interpreter.h"
#include "typedefs.h"
#include "memory_manager.h"
#include "statements.h"
#include <chrono>
#include <string>
#include <unordered_map>
#include <variant>

NAMESPACE_BEGIN(cpplox)

std::string cpplox_type_to_string(cpplox_type type)
{
    switch (type)
    {
        case cpplox_type::number_:     return "number";
        case cpplox_type::string_:     return "string";
        case cpplox_type::bool_:       return "bool";
        case cpplox_type::callable_:   return "callable";
        case cpplox_type::class_:      return "class";
        case cpplox_type::instance_:   return "instance";
        case cpplox_type::null_:       return "null";
        case cpplox_type::undefined_:  return "undefined";
    }

    return "unknown";
}

cpplox_type literal_to_cpplox_type(const literal_value& l)
{
    return std::visit(
        literal_value_overload{
            [](double)                      { return cpplox_type::number_;    },
            [](bool)                        { return cpplox_type::bool_;      },
            [](const std::string&)          { return cpplox_type::string_;    },
            [](const cpplox_callable*)         { return cpplox_type::callable_;  },
            [](const cpplox_class*)            { return cpplox_type::class_;     },
            [](const cpplox_instance*)         { return cpplox_type::instance_;  },
            [](std::monostate)              { return cpplox_type::null_;      },
            [](const undefined&)            { return cpplox_type::undefined_; },
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
            [&](const cpplox_callable* c)         { return c->to_string();                                 },
            [&](const cpplox_class* c)            { return c->name;                                        },
            [&](const cpplox_instance* i)         { return i->to_string();                                 },
            [&](std::monostate)                { return std::string("null");                            },
            [&](const undefined& u)            { return std::string("undefined");                       },
        }, l);
}

user_function::user_function(function_declaration_statement& declaration_,
        environment* closure_,
        environment_manager* env_manager,
        bool is_initializer)
    : declaration(declaration_)
    , closure(closure_)
    , _env_manager(env_manager)
    , _is_initializer(is_initializer) { }

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
    catch (const interpreter::cpplox_function_return& ret)
    {
        _env_manager->pop_environment();

        if (_is_initializer)
        {
            token this_token = token{ token_type::identifier_, "this", "", { 0, 0 }, "" };
            return closure->get(this_token);
        }

        return ret.return_val;
    }

    _env_manager->pop_environment();
    return std::monostate{};
}

cpplox_callable* user_function::bind(cpplox_instance* instance)
{
    environment* new_env = memory_manager::instance().allocate_environment(closure);
    new_env->define("this", instance);
    return memory_manager::instance().allocate_user_function(declaration, new_env, _env_manager, _is_initializer);
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

cpplox_class::cpplox_class(const std::string& name_, std::unordered_map<std::string, cpplox_callable*>&& methods_
        , cpplox_class* superclass_)
    : name(name_), methods(std::move(methods_)), superclass(superclass_) { }

int cpplox_class::arity() 
{ 
    cpplox_callable* initializer = find_method("init");
    if (initializer)
        return initializer->arity();

    return 0; 
}
std::string cpplox_class::to_string() const { return "<class>" + name; }

literal_value cpplox_class::call(interpreter& i, const std::vector<literal_value>& args)
{
    cpplox_instance* instance = memory_manager::instance().allocate_instance(this);
    cpplox_callable* initializer = find_method("init");

    if (initializer)
    {
        dynamic_cast<user_function*>(initializer)->bind(instance)->call(i, args);
    }

    return instance;
}

cpplox_callable* cpplox_class::find_method(const token& name)
{
    auto method_it = methods.find(name.lexeme);
    if (method_it != methods.end())
        return method_it->second;

    if (superclass != nullptr)
    {
        return superclass->find_method(name);
    }

    return nullptr;
}

cpplox_callable* cpplox_class::find_method(const std::string& name)
{
    auto method_it = methods.find(name);
    if (method_it != methods.end())
        return method_it->second;

    return nullptr;
}

cpplox_instance::cpplox_instance(cpplox_class* class_)
    : _class(class_) { }

std::string cpplox_instance::to_string() const
{
    return _class->name + " instance";
}

literal_value cpplox_instance::get(const token& name)
{
    auto field_it = _fields.find(name.lexeme);
    if (field_it != _fields.end())
        return field_it->second;

    cpplox_callable* method = _class->find_method(name);
    if (method)
    {
        user_function* user_method = dynamic_cast<user_function*>(method);
        return user_method->bind(this);
    }

    throw cpplox_runtime_error("Undefined property or method '" + name.lexeme + "'", name);
}

void cpplox_instance::set(const token& name, const literal_value& value)
{
    _fields[name.lexeme] = value;
}

NAMESPACE_END

#include "environment.h"
#include "geo_functions.h"
#include "typedefs.h"
#include "exceptions.h"
#include <memory>

NAMESPACE_BEGIN(geo)

environment_manager::environment::environment(environment* parent_scope)
    : _variables()
    , _parent_scope(parent_scope) { }

void environment_manager::environment::define(const std::string& name, const literal_value& value)
{
    auto find = _variables.find(name);
    if (find != _variables.end())
    {
        throw geo_runtime_error("Variable '" + name + "' already defined, did you mean to reassign it?");
    }
    _variables[name] = value;
}

void environment_manager::environment::assign(const std::string& name, const literal_value& value)
{
    auto find = _variables.find(name);
    if (find != _variables.end())
    {
        _variables[name] = value;
        return;
    }
    else
    {
        if (_parent_scope)
        {
            _parent_scope->assign(name, value);
            return;
        }
    }

    throw geo_runtime_error("Undefined variable '" + name + "' can not be assigned to");
}

literal_value environment_manager::environment::get(const token& name) const
{
    // Check for the variable name in the local lexical scope first
    auto it = _variables.find(name.lexeme);
    if (it != _variables.end())
    {
        geo_type type = literal_to_geo_type(it->second);
        if (type == geo_type::undefined_)
            throw geo_runtime_error("Variable '" + name.lexeme + "' is undefined", name);

        return it->second;
    }
    else
    {
        // If there's a parent scope to check, we check to see if that scope contains the variable to get
        if (_parent_scope)
            return _parent_scope->get(name);
    }

    // When no scopes have the variable, we found a runtime error
    throw geo_runtime_error("Undefined variable '" + name.lexeme + "'", name);
}

environment_manager::environment_manager()
    : _environments()
{
    _environments["global"] = new environment();
}

environment_manager::~environment_manager()
{
    for (auto& [name, env] : _environments)
        delete env;
}

environment_manager::environment* environment_manager::get_global_environment() const noexcept
{
    return _environments.at("global");
}

void environment_manager::push_environment(const std::string& name)
{
    _environments[name] = new environment(_environments["global"]);
}

void environment_manager::pop_environment()
{
    if (_environments.size() == 1)
        throw geo_runtime_error("Cannot pop global environment", token{});

    auto it = _environments.begin();
    delete it->second;

    _environments.erase(it);
}

NAMESPACE_END

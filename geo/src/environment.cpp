#include "environment.h"
#include "geo_types.h"
#include "typedefs.h"
#include "exceptions.h"
#include "memory_manager.h"
#include <vector>

NAMESPACE_BEGIN(geo)

environment::environment(environment* parent_scope)
    : _variables()
    , _parent_scope(parent_scope) { }

void environment::define(const std::string& name, const literal_value& value)
{
    auto find = _variables.find(name);
    if (find != _variables.end())
    {
        throw geo_runtime_error("Variable '" + name + "' already defined, did you mean to reassign it?");
    }
    _variables[name] = value;
}

void environment::assign(const std::string& name, const literal_value& value)
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

literal_value environment::get(const token& name) const
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
    _environments.emplace_back(memory_manager::instance().allocate_environment());
}

environment* environment_manager::get_global_environment() const noexcept
{
    return _environments.at(0);
}

environment* environment_manager::get_current_environment() const noexcept
{
    return _environments.back();
}

void environment_manager::push_environment()
{
    _environments.emplace_back(memory_manager::instance().allocate_environment(_environments.back()));
}

void environment_manager::push_environment(environment* parent_scope)
{
    _environments.emplace_back(memory_manager::instance().allocate_environment(parent_scope));
}

void environment_manager::pop_environment()
{
    if (_environments.size() == 1)
        throw geo_runtime_error("Cannot pop the global environment");

    _environments.pop_back();
}

void environment_manager::define(const std::string& name, const literal_value& value)
{
    _environments.back()->define(name, value);
}

void environment_manager::assign(const std::string& name, const literal_value& value)
{
    _environments.back()->assign(name, value);
}

void environment_manager::assign_at(int distance, const token& name, const literal_value& literal)
{
    return ancestor(distance)->assign(name.lexeme, literal);
}

literal_value environment_manager::get(const token& name) const
{
    return _environments.back()->get(name);
}

literal_value environment_manager::get_at(int distance, const token& name) const
{
    return ancestor(distance)->get(name);
}

environment* environment_manager::ancestor(int distance) const
{
    environment* env = _environments.back();
    for (int i = 0; i < distance; ++i)
    {
        env = env->_parent_scope;
    }

    return env;
}

NAMESPACE_END

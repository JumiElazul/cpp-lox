#include "environment.h"
#include "typedefs.h"
#include "exceptions.h"

NAMESPACE_BEGIN(lumina)

environment::environment()
    : _variables()
    , _enclosing_scope(nullptr)
{

}

environment::environment(environment* enclosing_scope)
    : _variables()
    , _enclosing_scope(std::move(enclosing_scope))
{

}

void environment::define(const std::string& name, const literal_value& value)
{
    auto find = _variables.find(name);
    if (find != _variables.end())
    {
        throw lumina_runtime_error("Variable '" + name + "' already defined, did you mean to reassign it?");
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
        if (_enclosing_scope)
        {
            _enclosing_scope->assign(name, value);
            return;
        }
    }

    throw lumina_runtime_error("Undefined variable '" + name + "' can not be assigned to");
}

literal_value environment::get(const token& name) const
{
    // Check for the variable name in the local lexical scope first
    auto it = _variables.find(name.lexeme);
    if (it != _variables.end())
    {
        return it->second;
    }
    else
    {
        // If there's a parent scope to check, we check to see if that scope contains the variable to get
        if (_enclosing_scope)
            return _enclosing_scope->get(name);
    }

    // When no scopes have the variable, we found a runtime error
    throw lumina_runtime_error("Undefined variable '" + name.lexeme + "' at position [" + std::to_string(name.position.first) + ":" + std::to_string(name.position.second) + "]");
}

NAMESPACE_END

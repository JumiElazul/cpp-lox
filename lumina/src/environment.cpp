#include "environment.h"
#include "typedefs.h"
#include "exceptions.h"

NAMESPACE_BEGIN(lumina)

environment::environment()
    : _variables()
{

}

void environment::define(const std::string& name, const literal_value& value)
{
    _variables[name] = value;
}

literal_value environment::get(const token& name) const
{
    auto it = _variables.find(name.lexeme);
    if (it != _variables.end())
    {
        return it->second;
    }

    throw lumina_runtime_error("Undefined variable '" + name.lexeme + "' at position [" + std::to_string(name.position.first) + ":" + std::to_string(name.position.second) + "]");
}

NAMESPACE_END

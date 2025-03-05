#include "memory_manager.h"
#include "environment.h"
#include "typedefs.h"
#include "geo_types.h"
#include <string>

NAMESPACE_BEGIN(geo)

memory_manager& memory_manager::instance()
{
    static memory_manager instance;
    return instance;
}

memory_manager::memory_manager()
    : _callables()
    , _instances()
    , _environments()
{ }

memory_manager::~memory_manager()
{
    for (auto callable : _callables)
        delete callable;

    for (auto instance : _instances)
        delete instance;

    for (auto environment : _environments)
        delete environment;
}

bool memory_manager::register_callable(geo_callable* callable)
{
    return _callables.insert(callable).second;
}

geo_callable* memory_manager::allocate_class(const std::string& name)
{
    geo_callable* new_class = new geo_class(name);
    _callables.insert(new_class);
    return new_class;
}

geo_callable* memory_manager::allocate_user_function(function_declaration_statement& stmt, environment* env, environment_manager* env_manager)
{
    geo_callable* new_function = new user_function(stmt, env, env_manager);
    _callables.insert(new_function);
    return new_function;
}

geo_instance* memory_manager::allocate_instance(geo_class* class_)
{
    geo_instance* new_instance = new geo_instance(class_);
    _instances.insert(new_instance);
    return new_instance;
}

environment* memory_manager::allocate_environment()
{
    environment* new_environment = new environment();
    _environments.insert(new_environment);
    return new_environment;
}

environment* memory_manager::allocate_environment(environment* parent_scope)
{
    environment* new_environment = new environment(parent_scope);
    _environments.insert(new_environment);
    return new_environment;
}

NAMESPACE_END

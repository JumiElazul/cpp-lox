#include "memory_manager.h"
#include "environment.h"
#include "typedefs.h"
#include "cpplox_types.h"
#include <string>

NAMESPACE_BEGIN(cpplox)

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

bool memory_manager::register_callable(cpplox_callable* callable)
{
    return _callables.insert(callable).second;
}

cpplox_callable* memory_manager::allocate_class(const std::string& name,
        std::unordered_map<std::string, cpplox_callable*>&& methods, cpplox_class* superclass)
{
    cpplox_callable* new_class = new cpplox_class(name, std::move(methods), superclass);
    _callables.insert(new_class);
    return new_class;
}

cpplox_callable* memory_manager::allocate_user_function(function_declaration_statement& stmt, environment* env, environment_manager* env_manager, bool is_initializer)
{
    cpplox_callable* new_function = new user_function(stmt, env, env_manager, is_initializer);
    _callables.insert(new_function);
    return new_function;
}

cpplox_instance* memory_manager::allocate_instance(cpplox_class* class_)
{
    cpplox_instance* new_instance = new cpplox_instance(class_);
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

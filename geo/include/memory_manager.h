#ifndef JUMI_GEO_MEMORY_MANAGER_H
#define JUMI_GEO_MEMORY_MANAGER_H
#include "statements.h"
#include "typedefs.h"
#include <string>
#include <unordered_set>

NAMESPACE_BEGIN(geo)

class geo_callable;
class geo_class;
class environment;
class environment_manager;
class function_declaration_statement;
class geo_instance;

class memory_manager
{
public:
    static memory_manager& instance();
    memory_manager();
    ~memory_manager();
    memory_manager(const memory_manager&) = delete;
    memory_manager& operator=(const memory_manager&) = delete;
    memory_manager(memory_manager&&) = delete;
    memory_manager& operator=(memory_manager&&) = delete;

    bool register_callable(geo_callable* callable);
    geo_callable* allocate_class(const std::string& name);
    geo_callable* allocate_user_function(function_declaration_statement& stmt, environment* env, environment_manager* env_manager);
    geo_instance* allocate_instance(geo_class* class_);
    environment* allocate_environment();
    environment* allocate_environment(environment* parent_scope);

private:
    std::unordered_set<geo_callable*> _callables;
    std::unordered_set<geo_instance*> _instances;
    std::unordered_set<environment*> _environments;
};

NAMESPACE_END

#endif

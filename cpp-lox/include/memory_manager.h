#ifndef JUMI_CPPLOX_MEMORY_MANAGER_H
#define JUMI_CPPLOX_MEMORY_MANAGER_H
#include "statements.h"
#include "typedefs.h"
#include <string>
#include <unordered_set>

NAMESPACE_BEGIN(cpplox)

class cpplox_callable;
class cpplox_class;
class environment;
class environment_manager;
class function_declaration_statement;
class cpplox_instance;

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

    bool register_callable(cpplox_callable* callable);
    cpplox_callable* allocate_class(const std::string& name,
            std::unordered_map<std::string, cpplox_callable*>&& methods, cpplox_class* superclass);
    cpplox_callable* allocate_user_function(function_declaration_statement& stmt,
            environment* env, environment_manager* env_manager, bool is_initializer = false);
    cpplox_instance* allocate_instance(cpplox_class* class_);
    environment* allocate_environment();
    environment* allocate_environment(environment* parent_scope);

private:
    std::unordered_set<cpplox_callable*> _callables;
    std::unordered_set<cpplox_instance*> _instances;
    std::unordered_set<environment*> _environments;
};

NAMESPACE_END

#endif

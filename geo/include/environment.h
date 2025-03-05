#ifndef JUMI_GEO_ENVIRONMENT_H
#define JUMI_GEO_ENVIRONMENT_H
#include "typedefs.h"
#include "geo_types.h"
#include "tokens.h"
#include <string>
#include <vector>

NAMESPACE_BEGIN(geo)

class environment
{
friend class environment_manager;
public:
    environment(environment* parent_scope = nullptr);

    void define(const std::string& name, const literal_value& value);
    void assign(const std::string& name, const literal_value& value);
    literal_value get(const token& name) const;

private:
    std::unordered_map<std::string, literal_value> _variables;
    environment* _parent_scope;
};

class environment_manager
{
public:
    environment_manager();

    [[nodiscard]] environment* get_global_environment() const noexcept;
    [[nodiscard]] environment* get_current_environment() const noexcept;
    void push_environment();
    void push_environment(environment* parent_scope);
    void pop_environment();
    void define(const std::string& name, const literal_value& value);
    void assign(const std::string& name, const literal_value& value);
    void assign_at(int distance, const token& name, const literal_value& literal);
    literal_value get(const token& name) const;
    literal_value get_at(int distance, const token& name) const;

private:
    std::vector<environment*> _environments;

    environment* ancestor(int distance) const;
};

NAMESPACE_END

#endif

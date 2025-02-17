#ifndef JUMI_GEO_ENVIRONMENT_H
#define JUMI_GEO_ENVIRONMENT_H
#include "typedefs.h"
#include "geo_types.h"
#include "tokens.h"
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(geo)

class environment_manager
{
    class environment
    {
    public:
        environment(environment* parent_scope = nullptr);

        void define(const std::string& name, const literal_value& value);
        void assign(const std::string& name, const literal_value& value);
        literal_value get(const token& name) const;

    private:
        std::unordered_map<std::string, literal_value> _variables;
        environment* _parent_scope;
    };

public:
    environment_manager();
    ~environment_manager();
    environment_manager(const environment_manager&) = delete;
    environment_manager& operator=(const environment_manager&) = delete;
    environment_manager(environment_manager&&) = delete;
    environment_manager& operator=(environment_manager&&) = delete;

    [[nodiscard]] environment* get_global_environment() const noexcept;
    void push_environment(const std::string& name);
    void pop_environment();
    void define(const std::string& name, const literal_value& value);
    void assign(const std::string& name, const literal_value& value);
    literal_value get(const token& name) const;

private:
    std::unordered_map<std::string, environment*> _environments;
};

NAMESPACE_END

#endif

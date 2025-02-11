#ifndef JUMI_GEO_ENVIRONMENT_H
#define JUMI_GEO_ENVIRONMENT_H
#include "typedefs.h"
#include "geo_types.h"
#include "tokens.h"
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(geo)

class environment
{
public:
    environment(environment* enclosing_scope = nullptr);

    void define(const std::string& name, const literal_value& value);
    void assign(const std::string& name, const literal_value& value);
    literal_value get(const token& name) const;

private:
    std::unordered_map<std::string, literal_value> _variables;
    environment* _enclosing_scope;
};

NAMESPACE_END

#endif

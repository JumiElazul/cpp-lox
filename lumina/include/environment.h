#ifndef JUMI_LUMINA_ENVIRONMENT_H
#define JUMI_LUMINA_ENVIRONMENT_H
#include "typedefs.h"
#include "lumina_types.h"
#include "tokens.h"
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

class environment
{
public:
    environment();

    void define(const std::string& name, const literal_value& value);
    void overwrite(const std::string& name, const literal_value& value);
    literal_value get(const token& name) const;

private:
    std::unordered_map<std::string, literal_value> _variables;
};

NAMESPACE_END

#endif

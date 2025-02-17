#ifndef JUMI_GEO_GEO_FUNCTIONS_H
#define JUMI_GEO_GEO_FUNCTIONS_H
#include "typedefs.h"
#include "geo_types.h"
#include <vector>

NAMESPACE_BEGIN(geo)

class interpreter;

class geo_callable
{
public:
    virtual ~geo_callable() = default;
    virtual int arity() = 0;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) = 0;
    virtual std::string to_string() const = 0;
};

NAMESPACE_END

#endif

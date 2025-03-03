#ifndef JUMI_GEO_GEO_CLASSES_H
#define JUMI_GEO_GEO_CLASSES_H
#include "typedefs.h"
#include <string>

NAMESPACE_BEGIN(geo)

class geo_class
{
public:
    std::string name;

    geo_class(const std::string& name_);
};

NAMESPACE_END

#endif

#ifndef JUMI_LUMINA_I_IO_H
#define JUMI_LUMINA_I_IO_H
#include "typedefs.h"
#include <initializer_list>
#include <string>

NAMESPACE_BEGIN(lumina)

class i_io
{
public:
    virtual ~i_io() = default;

    virtual void write(const std::string& message) const = 0;
    virtual void write(const std::initializer_list<std::string>& messages) const = 0;
    virtual std::string readline() const = 0;
};

NAMESPACE_END

#endif

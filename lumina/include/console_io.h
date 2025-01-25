#ifndef JUMI_LUMINA_CONSOLE_IO_H
#define JUMI_LUMINA_CONSOLE_IO_H
#include "typedefs.h"
#include "i_io.h"
#include <initializer_list>
#include <string>

NAMESPACE_BEGIN(lumina)

class console_io : public i_io
{
public:
    console_io();

    virtual void write(const std::string& message) const override;
    virtual void write(const std::initializer_list<std::string>& messages) const override;
    virtual std::string readline() const override;
};

NAMESPACE_END

#endif

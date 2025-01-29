#ifndef JUMI_LUMINA_CONSOLE_IO_H
#define JUMI_LUMINA_CONSOLE_IO_H
#include "typedefs.h"
#include <memory>
#include <string>

NAMESPACE_BEGIN(lumina)

struct token;
class console_io
{
public:
    console_io();
    ~console_io();

    std::string readline() const;

    std::ostream& out();
    std::ostream& err();

private:
    struct console_io_impl;
    std::unique_ptr<console_io_impl> _impl;
};

NAMESPACE_END

#endif

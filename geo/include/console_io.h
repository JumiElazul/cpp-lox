#ifndef JUMI_GEO_CONSOLE_IO_H
#define JUMI_GEO_CONSOLE_IO_H
#include "typedefs.h"
#include <memory>
#include <string>
#include <iostream>

NAMESPACE_BEGIN(geo)

#define MAX_HISTORY_SIZE 128

struct token;
class console_io
{
public:
    console_io();
    ~console_io();

    std::string readline(const char* msg) const;

    std::ostream& out();
    std::ostream& err();

private:
    struct console_io_impl;
    std::unique_ptr<console_io_impl> _impl;
};

NAMESPACE_END

#endif

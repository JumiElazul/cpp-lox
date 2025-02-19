#ifndef JUMI_GEO_GEO_APP_H
#define JUMI_GEO_GEO_APP_H
#include "typedefs.h"
#include "interpreter.h"
#include "resolver.h"
#include <memory>

NAMESPACE_BEGIN(geo)

class console_io;
class geo_app
{
public:
    geo_app();
    ~geo_app();

    void run_file_mode(const char* filepath);
    void run_interpreter_mode();

private:
    std::unique_ptr<console_io> _io;
    interpreter _interpreter;
    resolver _resolver;

    bool _had_runtime_error;

    void run(const std::string& source);
};

NAMESPACE_END

#endif

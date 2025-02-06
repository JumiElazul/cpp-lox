#ifndef JUMI_LUMINA_LUMINA_APP_H
#define JUMI_LUMINA_LUMINA_APP_H
#include "typedefs.h"
#include "environment.h"
#include "lexer.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

class console_io;
class lumina_app
{
public:
    lumina_app();
    ~lumina_app();

    void run_file_mode(const char* filepath);
    void run_interpreter_mode();

private:
    std::unique_ptr<console_io> _io;
    lexer _lexer;
    environment _env;

    bool _had_runtime_error;
};

NAMESPACE_END

#endif

#ifndef JUMI_CPPLOX_CPPLOX_APP_H
#define JUMI_CPPLOX_CPPLOX_APP_H
#include "typedefs.h"
#include "interpreter.h"
#include "resolver.h"
#include <memory>

NAMESPACE_BEGIN(cpplox)

class console_io;
class cpplox_app
{
public:
    cpplox_app();
    ~cpplox_app();
    cpplox_app(const cpplox_app& rhs) = delete;
    cpplox_app& operator=(const cpplox_app& rhs) = delete;
    cpplox_app(cpplox_app&& rhs) = delete;
    cpplox_app& operator=(cpplox_app&& rhs) = delete;

    void run_file_mode(const char* filepath);
    void run_interpreter_mode();

private:
    std::unique_ptr<console_io> _io;
    interpreter _interpreter;
    resolver _resolver;
    std::vector<std::unique_ptr<statement>> _statements;

    bool _had_runtime_error;

    void run(const std::string& source);
    void store_statements(std::vector<std::unique_ptr<statement>>&& statements);
};

NAMESPACE_END

#endif

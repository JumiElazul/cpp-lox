#ifndef JUMI_LUMINA_APPLICATION_H
#define JUMI_LUMINA_APPLICATION_H
#include "typedefs.h"
#include "lexer.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

class i_io;
class i_logger;

class application
{
public:
    application();
    ~application();

    void run();

private:
    std::unique_ptr<i_io> _io;
    lexer _lexer;
};

NAMESPACE_END
#endif

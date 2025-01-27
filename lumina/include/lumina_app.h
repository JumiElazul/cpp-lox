#ifndef JUMI_LUMINA_LUMINA_APP_H
#define JUMI_LUMINA_LUMINA_APP_H
#include "typedefs.h"
#include "lexer.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

class console_io;
class lumina_app
{
public:
    lumina_app();
    ~lumina_app();

    void run();

private:
    std::unique_ptr<console_io> _io;
    lexer _lexer;
};

NAMESPACE_END
#endif

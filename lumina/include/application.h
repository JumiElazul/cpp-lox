#ifndef JUMI_LUMINA_APPLICATION_H
#define JUMI_LUMINA_APPLICATION_H
#include "typedefs.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

class i_io;
class application
{
public:
    application();
    ~application();

    void run();

private:
    std::unique_ptr<i_io> _io;
};

NAMESPACE_END
#endif

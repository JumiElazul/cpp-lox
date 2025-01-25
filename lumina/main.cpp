#include "application.h"

namespace lumina
{
    int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        application app;
        app.run();

        return 0;
    }
}

int main(int argc, char* argv[])
{
    return lumina::main(argc, argv);
}

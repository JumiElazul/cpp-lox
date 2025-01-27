#include "lumina_app.h"

namespace lumina
{
    int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        lumina_app app;
        app.run();

        return 0;
    }
}

int main(int argc, char* argv[])
{
    return lumina::main(argc, argv);
}

#include "lumina_app.h"
#include <iostream>

namespace lumina
{
    int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        lumina_app app;

        if (argc >= 2)
        {
            app.run_file_mode(argv[1]);
        }
        else
        {
            app.run_interpreter_mode();
        }

        return 0;
    }
}

int main(int argc, char* argv[])
{
    return lumina::main(argc, argv);
}

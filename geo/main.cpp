#include "geo_app.h"

namespace geo
{
    int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        geo_app app;

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
    return geo::main(argc, argv);
}

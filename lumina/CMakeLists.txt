set(SOURCES
    "src/lumina_app.cpp"

    "src/console_io.cpp"
    "src/environment.cpp"
    "src/exceptions.cpp"
    "src/expressions.cpp"
    "src/expression_visitors.cpp"
    "src/interpreter.cpp"
    "src/lexer.cpp"
    "src/logger.cpp"
    "src/lumina_types.cpp"
    "src/parser.cpp"
    "src/statements.cpp"
    "src/tokens.cpp"
)

set(HEADERS
    "include/lumina_app.h"

    "include/i_logger.h"

    "include/console_io.h"
    "include/environment.h"
    "include/exceptions.h"
    "include/expressions.h"
    "include/expression_visitors.h"
    "include/interpreter.h"
    "include/lexer.h"
    "include/logger.h"
    "include/lumina_types.h"
    "include/parser.h"
    "include/statements.h"
    "include/statement_visitors.h"
    "include/tokens.h"

    "include/typedefs.h"
)

# Setup the core library
add_library(lumina-core STATIC ${SOURCES} ${HEADERS})
target_include_directories(lumina-core PUBLIC "include")

include(cmake/logging.cmake)

find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

target_link_libraries(lumina-core PRIVATE spdlog::spdlog)
target_compile_definitions(lumina-core PUBLIC "LUMINA_VERSION=\"${PROJECT_VERSION}\"")

# Setup the executable
add_executable(lumina "main.cpp")
target_link_libraries(lumina PRIVATE lumina-core)

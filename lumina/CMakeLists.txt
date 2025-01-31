set(SOURCES
    "src/lumina_app.cpp"

    "src/console_io.cpp"
    "src/exceptions.cpp"
    "src/expressions.cpp"
    "src/expression_visitors.cpp"
    "src/interpreter.cpp"
    "src/lexer.cpp"
    "src/logger.cpp"
    "src/lumina_types.cpp"
    "src/parser.cpp"
    "src/tokens.cpp"
)

set(HEADERS
    "include/lumina_app.h"

    "include/i_logger.h"

    "include/console_io.h"
    "include/exceptions.h"
    "include/expressions.h"
    "include/expression_visitors.h"
    "include/interpreter.h"
    "include/lexer.h"
    "include/logger.h"
    "include/lumina_types.h"
    "include/parser.h"
    "include/tokens.h"

    "include/typedefs.h"
)

# Setup the core library
add_library(lumina-core STATIC ${SOURCES} ${HEADERS})
target_include_directories(lumina-core PUBLIC "include")

set(LOG_LEVELS TRACE DEBUG INFO WARN ERROR CRITICAL OFF)
set(LOG_LEVEL "TRACE" CACHE STRING "The log level to use. Options are: ${LOG_LEVELS}")

list(FIND LOG_LEVELS ${LOG_LEVEL} LOG_LEVEL_INDEX)
if(LOG_LEVEL_INDEX EQUAL -1)
    message(FATAL_ERROR "Invalid log level: ${LOG_LEVEL}. Options are: ${LOG_LEVELS}")
endif()

if(LOG_LEVEL STREQUAL "TRACE")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_TRACE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
elseif(LOG_LEVEL STREQUAL "DEBUG")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_DEBUG SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
elseif (LOG_LEVEL STREQUAL "INFO")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_INFO SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
elseif (LOG_LEVEL STREQUAL "WARN")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_WARN SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN)
elseif (LOG_LEVEL STREQUAL "ERROR")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_ERROR SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
elseif (LOG_LEVEL STREQUAL "CRITICAL")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_CRITICAL SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_CRITICAL)
elseif (LOG_LEVEL STREQUAL "OFF")
    target_compile_definitions(lumina-core PUBLIC LUMINA_LOG_LEVEL_OFF SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF)
endif()

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(lumina-core PRIVATE spdlog::spdlog)

# Setup the executable
add_executable(lumina "main.cpp")
target_link_libraries(lumina PRIVATE lumina-core)


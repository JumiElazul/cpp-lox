project(lumina)

set(LUMINA_EXE_NAME "lumina")
set(LUMINA_CORE_NAME "lumina-core")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    "src/application.cpp"
    "src/console_io.cpp"
)

set(HEADERS
    "include/application.h"

    "include/console_io.h"
    "include/i_io.h"
    "include/typedefs.h"
)

# Setup the core library
add_library(${LUMINA_CORE_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${LUMINA_CORE_NAME} PUBLIC "include")

# Setup the executable
add_executable(${LUMINA_EXE_NAME} "main.cpp")
target_link_libraries(${LUMINA_EXE_NAME} PRIVATE ${LUMINA_CORE_NAME})

cmake_minimum_required(VERSION 3.21)

execute_process(
  COMMAND git describe --tags --always
  OUTPUT_VARIABLE GIT_VERSION_TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

option(GEO_ENABLE_TESTS "Enable building tests" ON)

include(cmake/git_version.cmake)
get_git_version(GIT_VERSION)

update_version_file(version.txt ${GIT_VERSION})
update_vcpkg_json(vcpkg.json ${GIT_VERSION})

project(geo VERSION ${GIT_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(geo)

if (GEO_ENABLE_TESTS)
    message(STATUS "GEO_ENABLE_TESTS was ON, building test executables")
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "GEO_ENABLE_TESTS was OFF, test executables will not be built")
endif()

cmake_minimum_required(VERSION 3.21)

execute_process(
  COMMAND git describe --tags --always
  OUTPUT_VARIABLE GIT_VERSION_TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

message("Git version tag: ${GIT_VERSION_TAG}")

string(REGEX REPLACE "v" "" GIT_VERSION ${GIT_VERSION_TAG})
string(REGEX REPLACE "-.*" "" GIT_VERSION ${GIT_VERSION})
message("Git version (stripped): ${GIT_VERSION}")

# --- Update version.txt ---

file(READ version.txt VERSION_FILE_CONTENT)
string(REGEX REPLACE "^[^\n]+" "${GIT_VERSION}" VERSION_FILE_CONTENT ${VERSION_FILE_CONTENT})
file(WRITE version.txt ${VERSION_FILE_CONTENT})

# --- Update vcpkg.json ---

file(READ vcpkg.json VCPKG_JSON_CONTENT)

set(VCPKG_REGEX_MATCH "\"version-string\": \"[0-9]+\\.[0-9]+\\.[0-9]+.*\",\n")
set(VCPKG_REPLACE_STRING "\"version-string\": \"${GIT_VERSION}\",\n")

string(REGEX REPLACE ${VCPKG_REGEX_MATCH} ${VCPKG_REPLACE_STRING} VCPKG_JSON_UPDATED ${VCPKG_JSON_CONTENT})
message(STATUS "Writing to vcpkg.json with project version: ${VCPKG_JSON_UPDATED}")
file(WRITE vcpkg.json "${VCPKG_JSON_UPDATED}")

# --- Set the project version ---

project(lumina VERSION ${GIT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Add subdirectories ---

add_subdirectory(lumina)

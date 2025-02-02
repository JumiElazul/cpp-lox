cmake_minimum_required(VERSION 3.21)

# Extract the version from the version.txt file
file(READ version.txt VERSION_FILE)
string(REGEX MATCH "^[^\n]+" LUMINA_VERSION "${VERSION_FILE}")
message(STATUS "Building Lumina version: ${LUMINA_VERSION}")

# Set the project version, and the vcpkg.json to that of the Lumina version
file(READ vcpkg.json VCPKG_JSON)

set(VCPKG_REGEX_MATCH    "\"version-string\": \"[0-9]+\.[0-9]+\.[0-9]+\"")
set(VCPKG_REPLACE_STRING "\"version-string\": \"${LUMINA_VERSION}\"")
string(REGEX REPLACE ${VCPKG_REGEX_MATCH} ${VCPKG_REPLACE_STRING} VCPKG_VERSION "${VCPKG_JSON}")

message(STATUS "Writing to vcpkg.json project version: ${VCPKG_VERSION}")
file(WRITE vcpkg.json "${VCPKG_VERSION}")

project(lumina VERSION ${LUMINA_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lumina)

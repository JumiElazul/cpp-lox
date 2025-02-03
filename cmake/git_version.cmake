function(get_git_version OUTPUT_VAR)
    execute_process(
        COMMAND git describe --tags --always
        OUTPUT_VARIABLE GIT_VERSION_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("Git version tag: ${GIT_VERSION_TAG}")
    string(REGEX REPLACE "v" "" GIT_VERSION ${GIT_VERSION_TAG})
    string(REGEX REPLACE "-.*" "" GIT_VERSION ${GIT_VERSION})
    set(${OUTPUT_VAR} ${GIT_VERSION} PARENT_SCOPE)
    message("Git version (stripped): ${GIT_VERSION}")
endfunction()

function(update_version_file FILE_PATH VERSION)
    message(STATUS "Writing to ${FILE_PATH} with project version: ${VERSION}")
    file(WRITE ${FILE_PATH} ${VERSION})
endfunction()

function(update_vcpkg_json FILE_PATH VERSION)
    file(READ ${FILE_PATH} VCPKG_JSON_CONTENT)

    set(VCPKG_REGEX_MATCH "\"version-string\": \"[A-Za-z0-9]+\\.?[A-Za-z0-9]+\\.?[A-Za-z0-9]+.*\",\n")
    set(VCPKG_REPLACE_STRING "\"version-string\": \"${VERSION}\",\n")

    string(REGEX REPLACE ${VCPKG_REGEX_MATCH} ${VCPKG_REPLACE_STRING} VCPKG_JSON_UPDATED ${VCPKG_JSON_CONTENT})
    message(STATUS "Writing to vcpkg.json: ${VCPKG_JSON_UPDATED}")
    file(WRITE ${FILE_PATH} "${VCPKG_JSON_UPDATED}")
endfunction()

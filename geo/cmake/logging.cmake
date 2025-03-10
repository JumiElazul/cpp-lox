option(GEO_ENABLE_CONSOLE_LOGGING "Enable logging to the console" ON)
option(GEO_ENABLE_FILE_LOGGING "Enable logging to the geo.log file" ON)

set(LOG_LEVELS TRACE DEBUG INFO WARN ERROR CRITICAL OFF)

set(GEO_CONSOLE_LOG_LEVEL INFO CACHE STRING "Log level for console. Options: ${LOG_LEVELS}")
set(GEO_FILE_LOG_LEVEL TRACE CACHE STRING "Log level for file. Options: ${LOG_LEVELS}")

foreach(var GEO_CONSOLE_LOG_LEVEL GEO_FILE_LOG_LEVEL)
    list(FIND LOG_LEVELS "${${var}}" _index)
    if(_index EQUAL -1)
        message(FATAL_ERROR "Invalid value for ${var}: ${${var}}. Options are: ${LOG_LEVELS}")
    endif()
endforeach()

# We always compile TRACE into SPDLOG at compile time, so that whatever we set the sink log levels to
# at runtime we can still get the messages.
target_compile_definitions(geo-core PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

function(set_spdlog_levels target console_level file_level)
    string(TOUPPER "${console_level}" console_level_upper)
    string(TOUPPER "${file_level}" file_level_upper)

    if(console_level_upper STREQUAL "TRACE")
        set(console_value 0)
    elseif(console_level_upper STREQUAL "DEBUG")
        set(console_value 1)
    elseif(console_level_upper STREQUAL "INFO")
        set(console_value 2)
    elseif(console_level_upper STREQUAL "WARN")
        set(console_value 3)
    elseif(console_level_upper STREQUAL "ERROR")
        set(console_value 4)
    elseif(console_level_upper STREQUAL "CRITICAL")
        set(console_value 5)
    elseif(console_level_upper STREQUAL "OFF")
        set(console_value 6)
    else()
        message(FATAL_ERROR "Invalid console log level: ${console_level}")
    endif()

    if(file_level_upper STREQUAL "TRACE")
        set(file_value 0)
    elseif(file_level_upper STREQUAL "DEBUG")
        set(file_value 1)
    elseif(file_level_upper STREQUAL "INFO")
        set(file_value 2)
    elseif(file_level_upper STREQUAL "WARN")
        set(file_value 3)
    elseif(file_level_upper STREQUAL "ERROR")
        set(file_value 4)
    elseif(file_level_upper STREQUAL "CRITICAL")
        set(file_value 5)
    elseif(file_level_upper STREQUAL "OFF")
        set(file_value 6)
    else()
        message(FATAL_ERROR "Invalid file log level: ${file_level}")
    endif()

    if (GEO_ENABLE_CONSOLE_LOGGING)
        target_compile_definitions(${target} PUBLIC 
            GEO_ENABLE_CONSOLE_LOGGING
            GEO_CONSOLE_LOG_LEVEL=${console_value})
    endif()

    if (GEO_ENABLE_FILE_LOGGING)
        target_compile_definitions(${target} PUBLIC
            GEO_ENABLE_FILE_LOGGING
            GEO_FILE_LOG_LEVEL=${file_value})
    endif()
endfunction()

set_spdlog_levels(geo-core ${GEO_CONSOLE_LOG_LEVEL} ${GEO_FILE_LOG_LEVEL})

if (GEO_ENABLE_CONSOLE_LOGGING)
    message(STATUS "Console logging enabled at level: ${GEO_CONSOLE_LOG_LEVEL}")
else()
    message(STATUS "Console logging disabled")
endif()
if (GEO_ENABLE_FILE_LOGGING)
    message(STATUS "File logging enabled at level: ${GEO_FILE_LOG_LEVEL}")
else()
    message(STATUS "File logging disabled")
endif()

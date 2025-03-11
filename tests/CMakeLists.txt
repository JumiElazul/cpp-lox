find_package(Catch2 CONFIG REQUIRED)

add_executable(lexer-tests "lexer_tests.cpp")
add_executable(parser-tests "parser_tests.cpp")

target_link_libraries(lexer-tests  PRIVATE cpp-lox-core Catch2::Catch2WithMain)
target_link_libraries(parser-tests PRIVATE cpp-lox-core Catch2::Catch2WithMain)

include(CTest)
include(Catch)

file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpplox)

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_test(NAME ${test_name} COMMAND cpp-lox ${test_file})
    message(STATUS "Adding test for ${test_file}")
endforeach()

catch_discover_tests(lexer-tests)
catch_discover_tests(parser-tests)

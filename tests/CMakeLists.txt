find_package(Catch2 CONFIG REQUIRED)

add_executable(lexer-tests "lexer_tests.cpp")
add_executable(parser-tests "parser_tests.cpp")

target_link_libraries(lexer-tests  PRIVATE lumina-core Catch2::Catch2WithMain)
target_link_libraries(parser-tests PRIVATE lumina-core Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(lexer-tests)
catch_discover_tests(parser-tests)

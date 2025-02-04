#include <catch2/catch_test_macros.hpp>
#include "typedefs.h"
#include "lexer_test_cases.h"
#include "lexer.h"

NAMESPACE_BEGIN(lumina)

TEST_CASE("Lexer handles empty input", "[lexer]") {
    lexer l;
    std::vector<token> tokens = l.tokenize("");
    REQUIRE(tokens.size() == 2);
}

TEST_CASE("Lexer always inserts beginning of file and end of file tokens", "[lexer]") {
    lexer l;

    SECTION("Empty input") {
        std::vector<token> tokens = l.tokenize("");
        REQUIRE(tokens.at(0).type == token_type::bof_);
        REQUIRE(tokens.at(1).type == token_type::eof_);
    }

    SECTION("Input with a single character") {
        std::vector<token> tokens = l.tokenize("a");
        REQUIRE(tokens.at(0).type == token_type::bof_);
        REQUIRE(tokens.at(1).type == token_type::identifier_);
        REQUIRE(tokens.at(2).type == token_type::eof_);
    }

    SECTION("Input with multiple characters") {
        std::vector<token> tokens = l.tokenize("var i = 1 + 5");
        size_t last = tokens.size() - 1;

        REQUIRE(tokens.at(0).type == token_type::bof_);
        REQUIRE(tokens.at(last).type == token_type::eof_);
    }
}

TEST_CASE("Lexer handles single character tokens", "[lexer]") {
    lexer l;

    SECTION("( token") {
        std::vector<token> tokens = l.tokenize("(");
        REQUIRE(tokens.at(1).type == token_type::left_paren_);
    }

    SECTION(") token") {
        std::vector<token> tokens = l.tokenize(")");
        REQUIRE(tokens.at(1).type == token_type::right_paren_);
    }

    SECTION("{ token") {
        std::vector<token> tokens = l.tokenize("{");
        REQUIRE(tokens.at(1).type == token_type::left_brace_);
    }

    SECTION("} token") {
        std::vector<token> tokens = l.tokenize("}");
        REQUIRE(tokens.at(1).type == token_type::right_brace_);
    }

    SECTION(", token") {
        std::vector<token> tokens = l.tokenize(",");
        REQUIRE(tokens.at(1).type == token_type::comma_);
    }

    SECTION(". token") {
        std::vector<token> tokens = l.tokenize(".");
        REQUIRE(tokens.at(1).type == token_type::dot_);
    }

    SECTION("; token") {
        std::vector<token> tokens = l.tokenize(";");
        REQUIRE(tokens.at(1).type == token_type::semicolon_);
    }

    SECTION(": token") {
        std::vector<token> tokens = l.tokenize(":");
        REQUIRE(tokens.at(1).type == token_type::colon_);
    }

    SECTION("? token") {
        std::vector<token> tokens = l.tokenize("?");
        REQUIRE(tokens.at(1).type == token_type::question_);
    }

}

TEST_CASE("Lexer handles one or two character tokens", "[lexer]") {
    lexer l;

    SECTION("- token") {
        std::vector<token> tokens = l.tokenize("-");
        REQUIRE(tokens.at(1).type == token_type::minus_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("-= token") {
        std::vector<token> tokens = l.tokenize("-=");
        REQUIRE(tokens.at(1).type == token_type::minus_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("+ token") {
        std::vector<token> tokens = l.tokenize("+");
        REQUIRE(tokens.at(1).type == token_type::plus_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("+= token") {
        std::vector<token> tokens = l.tokenize("+=");
        REQUIRE(tokens.at(1).type == token_type::plus_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("/ token") {
        std::vector<token> tokens = l.tokenize("/");
        REQUIRE(tokens.at(1).type == token_type::slash_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("/= token") {
        std::vector<token> tokens = l.tokenize("/=");
        REQUIRE(tokens.at(1).type == token_type::slash_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("* token") {
        std::vector<token> tokens = l.tokenize("*");
        REQUIRE(tokens.at(1).type == token_type::star_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("*= token") {
        std::vector<token> tokens = l.tokenize("*=");
        REQUIRE(tokens.at(1).type == token_type::star_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("! token") {
        std::vector<token> tokens = l.tokenize("!");
        REQUIRE(tokens.at(1).type == token_type::bang_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("!= token") {
        std::vector<token> tokens = l.tokenize("!=");
        REQUIRE(tokens.at(1).type == token_type::bang_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("= token") {
        std::vector<token> tokens = l.tokenize("=");
        REQUIRE(tokens.at(1).type == token_type::equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("== token") {
        std::vector<token> tokens = l.tokenize("==");
        REQUIRE(tokens.at(1).type == token_type::equal_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("> token") {
        std::vector<token> tokens = l.tokenize(">");
        REQUIRE(tokens.at(1).type == token_type::greater_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION(">= token") {
        std::vector<token> tokens = l.tokenize(">=");
        REQUIRE(tokens.at(1).type == token_type::greater_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("< token") {
        std::vector<token> tokens = l.tokenize("<");
        REQUIRE(tokens.at(1).type == token_type::less_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("<= token") {
        std::vector<token> tokens = l.tokenize("<=");
        REQUIRE(tokens.at(1).type == token_type::less_equal_);
        REQUIRE(tokens.size() == 3);
    }
}

TEST_CASE("Lexer correctly identifies all reserved keywords", "[lexer]") {
    lexer l;

    SECTION("and token") {
        std::vector<token> tokens = l.tokenize("and");
        REQUIRE(tokens.at(1).type == token_type::and_);
    }

    SECTION("or token") {
        std::vector<token> tokens = l.tokenize("or");
        REQUIRE(tokens.at(1).type == token_type::or_);
    }

    SECTION("if token") {
        std::vector<token> tokens = l.tokenize("if");
        REQUIRE(tokens.at(1).type == token_type::if_);
    }

    SECTION("else token") {
        std::vector<token> tokens = l.tokenize("else");
        REQUIRE(tokens.at(1).type == token_type::else_);
    }

    SECTION("class token") {
        std::vector<token> tokens = l.tokenize("class");
        REQUIRE(tokens.at(1).type == token_type::class_);
    }

    SECTION("false token") {
        std::vector<token> tokens = l.tokenize("false");
        REQUIRE(tokens.at(1).type == token_type::false_);
    }

    SECTION("true token") {
        std::vector<token> tokens = l.tokenize("true");
        REQUIRE(tokens.at(1).type == token_type::true_);
    }

    SECTION("func token") {
        std::vector<token> tokens = l.tokenize("func");
        REQUIRE(tokens.at(1).type == token_type::func_);
    }

    SECTION("null token") {
        std::vector<token> tokens = l.tokenize("null");
        REQUIRE(tokens.at(1).type == token_type::null_);
    }

    SECTION("print token") {
        std::vector<token> tokens = l.tokenize("print");
        REQUIRE(tokens.at(1).type == token_type::print_);
    }

    SECTION("return token") {
        std::vector<token> tokens = l.tokenize("return");
        REQUIRE(tokens.at(1).type == token_type::return_);
    }

    SECTION("super token") {
        std::vector<token> tokens = l.tokenize("super");
        REQUIRE(tokens.at(1).type == token_type::super_);
    }

    SECTION("this token") {
        std::vector<token> tokens = l.tokenize("this");
        REQUIRE(tokens.at(1).type == token_type::this_);
    }

    SECTION("var token") {
        std::vector<token> tokens = l.tokenize("var");
        REQUIRE(tokens.at(1).type == token_type::var_);
    }

    SECTION("for token") {
        std::vector<token> tokens = l.tokenize("for");
        REQUIRE(tokens.at(1).type == token_type::for_);
    }

    SECTION("while token") {
        std::vector<token> tokens = l.tokenize("while");
        REQUIRE(tokens.at(1).type == token_type::while_);
    }
}

TEST_CASE("Lexer correctly tokenizes various inputs", "[lexer]") {
    lexer l;

    for (const auto& test : lexer_test_cases) {
        DYNAMIC_SECTION("Input: " << test.input) {
            auto tokens = l.tokenize(test.input);

            INFO("Expected token count: " << test.expected_tokens.size()
                 << " but got: " << tokens.size());
            REQUIRE(tokens.size() == test.expected_tokens.size());

            for (std::size_t i = 0; i < tokens.size(); ++i) {
                INFO("At token index " << i);
                REQUIRE(tokens[i].type == test.expected_tokens[i]);
            }
        }
    }
}

NAMESPACE_END

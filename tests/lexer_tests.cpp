#include <catch2/catch_test_macros.hpp>
#include "typedefs.h"
#include "lexer_test_cases.h"
#include "lexer.h"

NAMESPACE_BEGIN(geo)

TEST_CASE("Lexer handles empty input", "[lexer]") {
    lexer l("");
    const std::vector<token>& tokens = l.get_tokens();
    REQUIRE(tokens.size() == 2);
}

TEST_CASE("Lexer always inserts beginning of file and end of file tokens", "[lexer]") {
    SECTION("Empty input") {
        lexer l("");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(0).type == token_type::bof_);
        REQUIRE(tokens.at(1).type == token_type::eof_);
    }

    SECTION("Input with a single character") {
        lexer l("a");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(0).type == token_type::bof_);
        REQUIRE(tokens.at(1).type == token_type::identifier_);
        REQUIRE(tokens.at(2).type == token_type::eof_);
    }

    SECTION("Input with multiple characters") {
        lexer l("var i = 1 + 5");
        const std::vector<token>& tokens = l.get_tokens();
        size_t last = tokens.size() - 1;

        REQUIRE(tokens.at(0).type == token_type::bof_);
        REQUIRE(tokens.at(last).type == token_type::eof_);
    }
}

TEST_CASE("Lexer handles single character tokens", "[lexer]") {
    SECTION("( token") {
        lexer l("(");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::left_paren_);
    }

    SECTION(") token") {
        lexer l(")");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::right_paren_);
    }

    SECTION("{ token") {
        lexer l("{");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::left_brace_);
    }

    SECTION("} token") {
        lexer l("}");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::right_brace_);
    }

    SECTION(", token") {
        lexer l(",");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::comma_);
    }

    SECTION(". token") {
        lexer l(".");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::dot_);
    }

    SECTION("; token") {
        lexer l(";");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::semicolon_);
    }

    SECTION(": token") {
        lexer l(":");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::colon_);
    }

    SECTION("? token") {
        lexer l("?");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::question_);
    }

}

TEST_CASE("Lexer handles one or two character tokens", "[lexer]") {
    SECTION("- token") {
        lexer l("-");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::minus_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("-= token") {
        lexer l("-=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::minus_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("+ token") {
        lexer l("+");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::plus_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("+= token") {
        lexer l("+=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::plus_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("/ token") {
        lexer l("/");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::slash_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("/= token") {
        lexer l("/=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::slash_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("* token") {
        lexer l("*");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::star_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("*= token") {
        lexer l("*=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::star_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("! token") {
        lexer l("!");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::bang_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("!= token") {
        lexer l("!=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::bang_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("= token") {
        lexer l("=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("== token") {
        lexer l("==");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::equal_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("> token") {
        lexer l(">");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::greater_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION(">= token") {
        lexer l(">=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::greater_equal_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("< token") {
        lexer l("<");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::less_);
        REQUIRE(tokens.size() == 3);
    }

    SECTION("<= token") {
        lexer l("<=");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::less_equal_);
        REQUIRE(tokens.size() == 3);
    }
}

TEST_CASE("Lexer correctly identifies all reserved keywords", "[lexer]") {
    SECTION("and token") {
        lexer l("and");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::and_);
    }

    SECTION("or token") {
        lexer l("or");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::or_);
    }

    SECTION("if token") {
        lexer l("if");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::if_);
    }

    SECTION("else token") {
        lexer l("else");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::else_);
    }

    SECTION("class token") {
        lexer l("class");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::class_);
    }

    SECTION("false token") {
        lexer l("false");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::false_);
    }

    SECTION("true token") {
        lexer l("true");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::true_);
    }

    SECTION("func token") {
        lexer l("func");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::func_);
    }

    SECTION("null token") {
        lexer l("null");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::null_);
    }

    SECTION("return token") {
        lexer l("return");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::return_);
    }

    SECTION("super token") {
        lexer l("super");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::super_);
    }

    SECTION("this token") {
        lexer l("this");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::this_);
    }

    SECTION("var token") {
        lexer l("var");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::var_);
    }

    SECTION("for token") {
        lexer l("for");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::for_);
    }

    SECTION("while token") {
        lexer l("while");
        const std::vector<token>& tokens = l.get_tokens();
        REQUIRE(tokens.at(1).type == token_type::while_);
    }
}

TEST_CASE("Lexer correctly get_tokenss various inputs", "[lexer]") {
    for (const auto& test : lexer_test_cases) {
        DYNAMIC_SECTION("Input: " << test.input) {
            lexer l(test.input);
            const std::vector<token>& tokens = l.get_tokens();

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

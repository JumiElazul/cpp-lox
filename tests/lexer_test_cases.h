#ifndef JUMI_GEO_TESTS_LEXER_TEST_CASES_H
#define JUMI_GEO_TESTS_LEXER_TEST_CASES_H
#include "typedefs.h"
#include "tokens.h"
#include <string>
#include <vector>

NAMESPACE_BEGIN(geo)

struct lexer_test_case {
    std::string input;
    std::vector<token_type> expected_tokens;
};

inline const std::vector<lexer_test_case> lexer_test_cases
{
    // --- Basic arithmetic expressions ---

    { "1 + 2",
        { token_type::bof_, token_type::number_, token_type::plus_, token_type::number_, token_type::eof_ } },

    { "4 - 2",
        { token_type::bof_, token_type::number_, token_type::minus_, token_type::number_, token_type::eof_ } },

    { "5 * 2",
        { token_type::bof_, token_type::number_, token_type::star_, token_type::number_, token_type::eof_ } },

    { "4 / 2",
        { token_type::bof_, token_type::number_, token_type::slash_, token_type::number_, token_type::eof_ } },

    { "(1 + 2) * 3",
        { token_type::bof_, token_type::left_paren_, token_type::number_, token_type::plus_,
            token_type::number_, token_type::right_paren_, token_type::star_, token_type::number_,
            token_type::eof_ } },


    // --- Grouping and punctuation ---

    { "(",
        { token_type::bof_, token_type::left_paren_, token_type::eof_ } },

    { ")",
        { token_type::bof_, token_type::right_paren_, token_type::eof_ } },

    { "{",
        { token_type::bof_, token_type::left_brace_, token_type::eof_ } },

    { "}",
        { token_type::bof_, token_type::right_brace_, token_type::eof_ } },

    { ",",
        { token_type::bof_, token_type::comma_, token_type::eof_ } },

    { ".",
        { token_type::bof_, token_type::dot_, token_type::eof_ } },

    { ";",
        { token_type::bof_, token_type::semicolon_, token_type::eof_ } },

    { ":",
        { token_type::bof_, token_type::colon_, token_type::eof_ } },

    { "?",
        { token_type::bof_, token_type::question_, token_type::eof_ } },


    // --- Two-character operators ---

    { "a -= 1",
        { token_type::bof_, token_type::identifier_, token_type::minus_equal_, token_type::number_,
            token_type::eof_ } },

    { "b += 2",
        { token_type::bof_, token_type::identifier_, token_type::plus_equal_, token_type::number_,
            token_type::eof_ } },

    { "c *= 3",
        { token_type::bof_, token_type::identifier_, token_type::star_equal_, token_type::number_,
            token_type::eof_ } },

    { "d /= 4",
        { token_type::bof_, token_type::identifier_, token_type::slash_equal_, token_type::number_,
            token_type::eof_ } },

    { "!=",
        { token_type::bof_, token_type::bang_equal_, token_type::eof_ } },

    { "==",
        { token_type::bof_, token_type::equal_equal_, token_type::eof_ } },

    { ">=",
        { token_type::bof_, token_type::greater_equal_, token_type::eof_ } },

    { "<=",
        { token_type::bof_, token_type::less_equal_, token_type::eof_ } },


    // --- Single-character operators ---

    { "!",
        { token_type::bof_, token_type::bang_, token_type::eof_ } },

    { "=",
        { token_type::bof_, token_type::equal_, token_type::eof_ } },

    { "+",
        { token_type::bof_, token_type::plus_, token_type::eof_ } },

    { "-",
        { token_type::bof_, token_type::minus_, token_type::eof_ } },

    { "*",
        { token_type::bof_, token_type::star_, token_type::eof_ } },

    { "/",
        { token_type::bof_, token_type::slash_, token_type::eof_ } },

    { ">",
        { token_type::bof_, token_type::greater_, token_type::eof_ } },

    { "<",
        { token_type::bof_, token_type::less_, token_type::eof_ } },


    // --- Keywords and reserved keywords ---

    { "var x = 10",
        { token_type::bof_, token_type::var_, token_type::identifier_, token_type::equal_,
            token_type::number_, token_type::eof_ } },

    { "if (x > 3) { print(x); }",
        { token_type::bof_, token_type::if_, token_type::left_paren_, token_type::identifier_,
            token_type::greater_, token_type::number_, token_type::right_paren_,
            token_type::left_brace_, token_type::identifier_, token_type::left_paren_,
            token_type::identifier_, token_type::right_paren_, token_type::semicolon_,
            token_type::right_brace_, token_type::eof_ } },

    { "and or if else class false true func null print return super this var for while",
        { token_type::bof_, token_type::and_, token_type::or_, token_type::if_, token_type::else_,
            token_type::class_, token_type::false_, token_type::true_, token_type::func_, token_type::null_,
            token_type::identifier_, token_type::return_, token_type::super_, token_type::this_,
            token_type::var_, token_type::for_, token_type::while_, token_type::eof_ } },


    // --- Literals ---

    { "\"Hello, world!\"",
        { token_type::bof_, token_type::string_, token_type::eof_ } },

    { "foo \"bar baz\" qux",
        { token_type::bof_, token_type::identifier_, token_type::string_,
            token_type::identifier_, token_type::eof_ } },

    { "123",
        { token_type::bof_, token_type::number_, token_type::eof_ } },

    { "3.14",
        { token_type::bof_, token_type::number_, token_type::eof_ } },


    // --- Identifiers ---

    { "foo",
        { token_type::bof_, token_type::identifier_, token_type::eof_ } },

    { "foo_bar",
        { token_type::bof_, token_type::identifier_, token_type::eof_ } },

    { "foo123",
        { token_type::bof_, token_type::identifier_, token_type::eof_ } },

    { "_foo",
        { token_type::bof_, token_type::identifier_, token_type::eof_ } },

    { "_foo_bar",
        { token_type::bof_, token_type::identifier_, token_type::eof_ } },

    { "____",
        { token_type::bof_, token_type::identifier_, token_type::eof_ } },


    // --- Invalid tokens and Whitespace ---

    { "@",
        { token_type::bof_, token_type::eof_ } },


};

NAMESPACE_END

#endif


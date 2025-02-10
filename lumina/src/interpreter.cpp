#include "interpreter.h"
#include "console_io.h"
#include "environment.h"
#include "exceptions.h"
#include "expressions.h"
#include "lumina_types.h"
#include "tokens.h"
#include "typedefs.h"
#include "statements.h"
#include <cassert>
#include <memory>
#include <string>
#include <vector>

NAMESPACE_BEGIN(lumina)

interpreter::interpreter(console_io* io)
    : _env(new environment())
    , _io(io)
    , _print_expr(false)
{

}

void interpreter::interpret(const std::vector<std::unique_ptr<statement>>& statements, bool print_expr)
{
    _print_expr = print_expr;

    try
    {
        for (const auto& stmt : statements)
        {
            stmt->accept_visitor(*this);
        }
    }
    catch (const lumina_runtime_error& e)
    {
        _io->err() << e.what() << '\n';
    }
}

void interpreter::visit_print_statement(print_statement& stmt)
{
    literal_value literal = stmt.expr->accept_visitor(*this);
    _io->out() << literal_tostr(literal) << '\n';
}

void interpreter::visit_if_statement(if_statement& stmt)
{
    if (is_truthy(stmt.condition->accept_visitor(*this)))
    {
        stmt.if_branch->accept_visitor(*this);
    }
    else
    {
        stmt.else_branch->accept_visitor(*this);
    }
}

void interpreter::visit_block_statement(block_statement& stmt)
{
    std::unique_ptr<environment> previous_env = std::move(_env);
    _env = std::make_unique<environment>(previous_env.get());

    try
    {
        for (const auto& s : stmt.statements)
        {
            s->accept_visitor(*this);
        }
    }
    catch (const lumina_runtime_error& e)
    {
        _io->err() << e.what() << '\n';
    }

    _env = std::move(previous_env);
}

void interpreter::visit_expression_statement(expression_statement& stmt)
{
    literal_value literal = stmt.expr->accept_visitor(*this);
    if (_print_expr)
    {
        _io->out() << literal_tostr(literal) << '\n';
    }
}

void interpreter::visit_variable_declaration_statement(variable_declaration_statement& stmt)
{
    if (stmt.initializer_expr)
    {
        literal_value literal = stmt.initializer_expr->accept_visitor(*this);
        _env->define(stmt.ident_name.lexeme, literal);
    }
    else
    {
        _env->define(stmt.ident_name.lexeme, undefined{});
    }
}

literal_value interpreter::visit_unary(unary_expression& expr)
{
    literal_value literal = expr.expr_rhs->accept_visitor(*this);
    lumina_type literal_type = literal_to_lumina_type(literal);

    const token& oper = expr.oper;

    if (oper.type == token_type::bang_)
    {
        if (literal_type != lumina_type::bool_)
            throw type_error("Cannot use unary operator ('!') on non-bool type", oper);

        return !is_truthy(literal);
    }
    else if (oper.type == token_type::minus_)
    {
        if (literal_type != lumina_type::number_)
            throw type_error("Cannot use unary operator ('-') on non-number type", oper);

        return -(std::get<double>(literal));
    }

    throw lumina_type_error("Unknown unary operator", oper);
}

literal_value interpreter::visit_binary(binary_expression& expr)
{
    literal_value lhs = expr.expr_lhs->accept_visitor(*this);
    literal_value rhs = expr.expr_rhs->accept_visitor(*this);

    lumina_type lhs_type = literal_to_lumina_type(lhs);
    lumina_type rhs_type = literal_to_lumina_type(rhs);

    const token& oper = expr.oper;

    if (lhs_type != rhs_type)
    {
        std::string msg = std::string("Cannot use binary operator '" + oper.lexeme + "' on types " +
                lumina_type_tostr(lhs_type) + " and " + lumina_type_tostr(rhs_type));

        throw lumina_type_error(msg, oper);
    }

    switch (oper.type)
    {
        case token_type::plus_:
        {
            if (lhs_type == lumina_type::string_)
            {
                return std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }
            else if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) + std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '+'", oper);
            }
        } break;
        case token_type::minus_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) - std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '-'", oper);
            }
        } break;
        case token_type::star_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) * std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '*'", oper);
            }
        } break;
        case token_type::slash_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) / std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '/'", oper);
            }
        } break;
        case token_type::greater_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) > std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '>'", oper);
            }
        } break;
        case token_type::greater_equal_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) >= std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '>='", oper);
            }
        } break;
        case token_type::less_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) < std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '<'", oper);
            }
        } break;
        case token_type::less_equal_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) <= std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '<='", oper);
            }
        } break;
        case token_type::bang_equal_:
        {
            return !is_equal(lhs, rhs);
        } break;
        case token_type::equal_equal_:
        {
            return is_equal(lhs, rhs);
        } break;
        default:
        {
            throw type_error("Unknown operator in handle_binary()", oper);
        };
    }
    throw type_error("Unknown operator in handle_binary()", oper);
}

literal_value interpreter::visit_ternary(ternary_expression& expr)
{
    literal_value if_literal = expr.expr_lhs->accept_visitor(*this);
    literal_value then_literal = expr.expr_then->accept_visitor(*this);
    literal_value else_literal = expr.expr_else->accept_visitor(*this);

    lumina_type if_type = literal_to_lumina_type(if_literal);
    const token& oper = expr.oper;

    if (if_type != lumina_type::bool_)
    {
        throw lumina_runtime_error("Cannot convert lhs of ternary expression to bool", oper);
    }

    return std::get<bool>(if_literal) ? then_literal : else_literal;
}

literal_value interpreter::visit_literal(literal_expression& expr)
{
    return expr.literal_val;
}

literal_value interpreter::visit_grouping(grouping_expression& expr)
{
    return expr.expr_lhs->accept_visitor(*this);
}

literal_value interpreter::visit_variable(variable_expression& expr)
{
    return _env->get(expr.ident_name);
}

literal_value interpreter::visit_logical(logical_expression& expr)
{
    if (expr.oper.type == token_type::or_)
    {

    }
    else if (expr.oper.type == token_type::and_)
    {

    }
}

literal_value interpreter::visit_assignment(assignment_expression& expr)
{
    literal_value literal = expr.initializer_expr->accept_visitor(*this);
    _env->assign(expr.ident_name.lexeme, literal);
    return literal;
}

bool interpreter::is_truthy(const literal_value& literal) const
{
    lumina_type type = literal_to_lumina_type(literal);

    switch (type)
    {
        case lumina_type::bool_:
        {
            return std::get<bool>(literal);
        } break;
        case lumina_type::number_:
        {
            return std::get<double>(literal) != 0;
        } break;
        case lumina_type::string_:
        {
            return !std::get<std::string>(literal).empty();
        } break;
        case lumina_type::null_:
        {
            return false;
        } break;
    }
    throw lumina_runtime_error("Unknown type in is_truthy()");
}

bool interpreter::is_equal(const literal_value& lhs, const literal_value& rhs) const
{
    lumina_type lhs_type = literal_to_lumina_type(lhs);
    lumina_type rhs_type = literal_to_lumina_type(rhs);

    if (lhs_type != rhs_type)
    {
        return false;
    }

    if ((lhs_type == lumina_type::null_) && (rhs_type == lumina_type::null_))
        return true;
    if (lhs_type == lumina_type::null_)
        return false;

    return lhs == rhs;
}

lumina_type_error interpreter::type_error(const std::string& msg, const token& t) const
{
    return lumina_type_error(msg, t);
}

NAMESPACE_END

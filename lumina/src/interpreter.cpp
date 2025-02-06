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

interpreter::interpreter(environment& env, console_io* io)
    : _env(env)
    , _io(io)
{

}

void interpreter::interpret(const std::vector<std::unique_ptr<statement>>& statements)
{
    try
    {
        for (const auto& stmt : statements)
        {
            stmt->accept_visitor(*this);
        }
    }
    catch (const lumina_type_error& e)
    {
        assert(false);
    }
    catch (const lumina_runtime_error& e)
    {
        assert(false);
    }
}

void interpreter::visit_print_statement(const print_statement& stmt) const
{
    literal_value literal = stmt.expr->accept_visitor(*this);
    _io->out() << literal_tostr(literal) << '\n';
}

void interpreter::visit_expression_statement(const expression_statement& stmt) const
{
    literal_value literal = stmt.expr->accept_visitor(*this);
}

void interpreter::visit_variable_declaration_statement(const variable_declaration_statement& stmt) const
{
    if (stmt.expr)
    {
        literal_value literal = stmt.expr->accept_visitor(*this);
        _env.define(stmt.ident_name.lexeme, literal);
    }
    else
    {
        _env.define(stmt.ident_name.lexeme, std::monostate{});
    }
}

literal_value interpreter::visit_unary(const unary_expression& expr) const
{
    literal_value literal = expr.expr_rhs->accept_visitor(*this);
    return handle_unary(expr.oper, literal);
}

literal_value interpreter::visit_binary(const binary_expression& expr) const
{
    literal_value lhs = expr.expr_lhs->accept_visitor(*this);
    literal_value rhs = expr.expr_rhs->accept_visitor(*this);

    return handle_binary(lhs, expr.oper, rhs);
}

literal_value interpreter::visit_ternary(const ternary_expression& expr) const
{
    literal_value if_expr = expr.expr_lhs->accept_visitor(*this);
    literal_value then_expr = expr.expr_then->accept_visitor(*this);
    literal_value else_expr = expr.expr_else->accept_visitor(*this);

    return handle_ternary(if_expr, expr.oper, then_expr, else_expr);
}

literal_value interpreter::visit_literal(const literal_expression& expr) const
{
    return expr.literal_val;
}

literal_value interpreter::visit_grouping(const grouping_expression& expr) const
{
    return expr.expr_lhs->accept_visitor(*this);
}

literal_value interpreter::visit_variable(const variable_expression& expr) const
{
    return _env.get(expr.ident_name);
}

literal_value interpreter::handle_unary(const token& oper, const literal_value& literal) const
{
    lumina_type literal_type = literal_to_lumina_type(literal);

    if (oper.type == token_type::bang_)
    {
        if (literal_type != lumina_type::bool_)
            throw lumina_type_error("Cannot use unary operator ('!') on non-bool type", oper);

        return !is_truthy(literal);
    }
    else if (oper.type == token_type::minus_)
    {
        if (literal_type != lumina_type::number_)
            throw lumina_type_error("Cannot use unary operator ('-') on non-number type", oper);

        return -(std::get<double>(literal));
    }
    throw lumina_runtime_error("Unknown operator in visit_unary()");
}

literal_value interpreter::handle_binary(const literal_value& lhs, const token& oper, const literal_value& rhs) const
{
    lumina_type lhs_type = literal_to_lumina_type(lhs);
    lumina_type rhs_type = literal_to_lumina_type(rhs);

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
                throw lumina_type_error("Unsupported type for binary operator '+'", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '-'", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '*'", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '/'", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '>'", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '>='", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '<'", oper);
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
                throw lumina_type_error("Unsupported type for binary operator '<='", oper);
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
            throw lumina_runtime_error("Unknown operator in handle_binary()");
        };
    }
    throw lumina_runtime_error("Unknown operator in handle_binary()");
}

literal_value interpreter::handle_ternary(const literal_value& if_literal, const token& oper, const literal_value& then_literal, const literal_value& else_literal) const
{
    lumina_type if_type = literal_to_lumina_type(if_literal);

    if (if_type != lumina_type::bool_)
    {
        throw lumina_runtime_error("Cannot convert lhs of ternary expression to bool");
    }

    return std::get<bool>(if_literal) ? then_literal : else_literal;
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

    // Disallow different types from being compared for now
    if (lhs_type != rhs_type)
    {
        return false;
    }

    if ((lhs_type == lumina_type::null_) && (rhs_type == lumina_type::null_))
        return true;
    if (lhs_type == lumina_type::null_)
        return false;

    // TODO: Figure out if this is correct
    return lhs == rhs;
}

NAMESPACE_END

#include "interpreter.h"
#include "console_io.h"
#include "environment.h"
#include "exceptions.h"
#include "expressions.h"
#include "geo_types.h"
#include "geo_native_funcs.h"
#include "tokens.h"
#include "typedefs.h"
#include "statements.h"
#include <cassert>
#include <memory>
#include <string>
#include <vector>

NAMESPACE_BEGIN(geo)

interpreter::interpreter(console_io* io)
    : _globals(std::make_unique<environment>())
    , _curr_env(_globals.get())
    , _io(io)
{
    instantiate_native_funcs();
}

interpreter::~interpreter()
{
    for (const auto& [name, value] : _globals->_variables)
    {
        if (literal_to_geo_type(value) == geo_type::callable_)
        {
            delete std::get<geo_callable*>(value);
        }
    }
}

void interpreter::interpret(const std::vector<std::unique_ptr<statement>>& statements)
{
    try
    {
        for (const auto& stmt : statements)
        {
            evaluate(stmt);
        }
    }
    catch (const geo_runtime_error& e)
    {
        _io->err() << e.what() << '\n';
    }
}

environment* interpreter::global_environment() const { return _globals.get(); }

void interpreter::instantiate_native_funcs()
{
    _globals->define("clock", new clock());
    _globals->define("print", new print(_io));
    _globals->define("input", new input(_io));
    _globals->define("random", new random());
}

literal_value interpreter::evaluate(const std::unique_ptr<expression>& expr)
{
    return expr->accept_visitor(*this);
}

void interpreter::evaluate(const std::unique_ptr<statement>& stmt)
{
    stmt->accept_visitor(*this);
}

void interpreter::execute_block(const std::vector<std::unique_ptr<statement>>& statements, environment* new_environment)
{
    environment* previous_env = _curr_env;
    _curr_env = new_environment;

    try
    {
        for (const auto& s : statements)
        {
            evaluate(s);
        }
    }
    catch (const geo_loop_break&)
    {
        _curr_env = std::move(previous_env);
        throw;
    }
    catch (const geo_loop_continue&)
    {
        _curr_env = std::move(previous_env);
        throw;
    }
    catch (const geo_runtime_error& e)
    {
        _io->err() << e.what() << '\n';
    }

    _curr_env = previous_env;
}

void interpreter::visit_function_declaration_statement(function_declaration_statement& stmt)
{
    geo_function* func = new geo_function(&stmt);
    _curr_env->define(stmt.ident_name.lexeme, func);
}

void interpreter::visit_variable_declaration_statement(variable_declaration_statement& stmt)
{
    if (stmt.initializer_expr)
    {
        literal_value literal = evaluate(stmt.initializer_expr);
        _curr_env->define(stmt.ident_name.lexeme, literal);
    }
    else
    {
        _curr_env->define(stmt.ident_name.lexeme, undefined{});
    }
}

void interpreter::visit_print_statement(print_statement& stmt)
{
    literal_value literal = evaluate(stmt.expr);
    _io->out() << literal_tostr(literal) << '\n';
}

void interpreter::visit_if_statement(if_statement& stmt)
{
    if (is_truthy(evaluate(stmt.condition)))
    {
        evaluate(stmt.if_branch);
    }
    else
    {
        // Can be null
        if (stmt.else_branch)
        {
            evaluate(stmt.else_branch);
        }
    }
}

void interpreter::visit_while_statement(while_statement& stmt)
{
    while (is_truthy(evaluate(stmt.condition)))
    {
        try
        {
            evaluate(stmt.stmt_body);
        }
        catch (const geo_loop_break&)
        {
            break;
        }
        catch (const geo_loop_continue&)
        {
            continue;
        }
    }
}

void interpreter::visit_for_statement(for_statement& stmt)
{
    environment* previous_env = _curr_env;
    std::unique_ptr<environment> block_env = std::make_unique<environment>(previous_env);
    _curr_env = block_env.get();

    if (stmt.initializer)
    {
        evaluate(stmt.initializer);
    }

    while (is_truthy(evaluate(stmt.condition)))
    {
        try
        {
            evaluate(stmt.stmt_body);
        }
        catch (const geo_loop_break&)
        {
            break;
        }
        catch (const geo_loop_continue&)
        {
            if (stmt.increment)
                evaluate(stmt.increment);

            continue;
        }

        if (stmt.increment)
            evaluate(stmt.increment);
    }

    _curr_env = previous_env;
}

void interpreter::visit_break_statement(break_statement& stmt)
{
    throw geo_loop_break();
}

void interpreter::visit_continue_statement(continue_statement& stmt)
{
    throw geo_loop_continue();
}

void interpreter::visit_block_statement(block_statement& stmt)
{
    std::unique_ptr<environment> new_environment = std::make_unique<environment>(_curr_env);
    execute_block(stmt.statements, new_environment.get());
}

void interpreter::visit_expression_statement(expression_statement& stmt)
{
    literal_value literal = evaluate(stmt.expr);
}

literal_value interpreter::visit_unary(unary_expression& expr)
{
    literal_value literal = evaluate(expr.expr_rhs);
    geo_type literal_type = literal_to_geo_type(literal);

    const token& oper = expr.oper;

    if (oper.type == token_type::bang_)
    {
        if (literal_type != geo_type::bool_)
            throw type_error("Cannot use unary operator ('!') on non-bool type", oper);

        return !is_truthy(literal);
    }

    if (oper.type == token_type::minus_)
    {
        if (literal_type != geo_type::number_)
            throw type_error("Cannot use unary operator ('-') on non-number type", oper);

        return -(std::get<double>(literal));
    }

    if (oper.type == token_type::plus_plus_ || oper.type == token_type::minus_minus_)
    {
        variable_expression* var_expr = dynamic_cast<variable_expression*>(expr.expr_rhs.get());
        std::string oper = expr.oper.lexeme;

        if (!var_expr)
            throw type_error("Unary prefix operator '" + oper + "' requires a variable operand", expr.oper);

        literal_value literal = _curr_env->get(var_expr->ident_name);
        geo_type type = literal_to_geo_type(literal);

        if (type != geo_type::number_)
            throw type_error("Invalid type for postfix operator '" + oper + "'", expr.oper);

        double value = std::get<double>(literal);

        if (expr.oper.type == token_type::plus_plus_)
            ++value;
        else if (expr.oper.type == token_type::minus_minus_)
            --value;

        _curr_env->assign(var_expr->ident_name.lexeme, value);
        return value;
    }

    throw geo_type_error("Unknown unary operator", oper);
}

literal_value interpreter::visit_binary(binary_expression& expr)
{
    literal_value lhs = evaluate(expr.expr_lhs);
    literal_value rhs = evaluate(expr.expr_rhs);

    geo_type lhs_type = literal_to_geo_type(lhs);
    geo_type rhs_type = literal_to_geo_type(rhs);

    const token& oper = expr.oper;

    if (lhs_type != rhs_type)
    {
        std::string msg = std::string("Cannot use binary operator '" + oper.lexeme + "' on types " +
                geo_type_tostr(lhs_type) + " and " + geo_type_tostr(rhs_type));

        throw geo_type_error(msg, oper);
    }

    switch (oper.type)
    {
        case token_type::plus_:
        {
            if (lhs_type == geo_type::string_)
            {
                return std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }
            else if (lhs_type == geo_type::number_)
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
            if (lhs_type == geo_type::number_)
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
            if (lhs_type == geo_type::number_)
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
            if (lhs_type == geo_type::number_)
            {
                return std::get<double>(lhs) / std::get<double>(rhs);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '/'", oper);
            }
        } break;
        case token_type::modulo_:
        {
            if (lhs_type == geo_type::number_)
            {
                int64 lhs_long = static_cast<int64>(std::get<double>(lhs));
                int64 rhs_long = static_cast<int64>(std::get<double>(rhs));
                return static_cast<double>(lhs_long % rhs_long);
            }
            else
            {
                throw type_error("Unsupported type for binary operator '/'", oper);
            }
        } break;
        case token_type::greater_:
        {
            if (lhs_type == geo_type::number_)
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
            if (lhs_type == geo_type::number_)
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
            if (lhs_type == geo_type::number_)
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
            if (lhs_type == geo_type::number_)
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
    literal_value if_literal = evaluate(expr.expr_lhs);
    literal_value then_literal = evaluate(expr.expr_then);
    literal_value else_literal = evaluate(expr.expr_else);

    geo_type if_type = literal_to_geo_type(if_literal);
    const token& oper = expr.oper;

    if (if_type != geo_type::bool_)
    {
        throw geo_runtime_error("Cannot convert lhs of ternary expression to bool", oper);
    }

    return std::get<bool>(if_literal) ? then_literal : else_literal;
}

literal_value interpreter::visit_literal(literal_expression& expr)
{
    return expr.literal_val;
}

literal_value interpreter::visit_grouping(grouping_expression& expr)
{
    return evaluate(expr.expr_);
}

literal_value interpreter::visit_variable(variable_expression& expr)
{
    return _curr_env->get(expr.ident_name);
}

literal_value interpreter::visit_assignment(assignment_expression& expr)
{
    literal_value literal = evaluate(expr.initializer_expr);
    _curr_env->assign(expr.ident_name.lexeme, literal);
    return literal;
}

literal_value interpreter::visit_logical(logical_expression& expr)
{
    literal_value lhs = evaluate(expr.expr_lhs);

    if (expr.oper.type == token_type::or_) 
    {
        if (is_truthy(lhs)) return lhs;
    }
    else 
    {
        if (!is_truthy(lhs)) return lhs;
    }

    return evaluate(expr.expr_rhs);
}

literal_value interpreter::visit_postfix(postfix_expression& expr)
{
    variable_expression* var_expr = dynamic_cast<variable_expression*>(expr.expr_lhs.get());
    std::string oper = expr.oper.lexeme;

    if (!var_expr)
        throw type_error("Postfix operator '" + oper + "' requires a variable operand", expr.oper);

    literal_value literal = _curr_env->get(var_expr->ident_name);
    geo_type type = literal_to_geo_type(literal);

    if (type != geo_type::number_)
        throw type_error("Invalid type for postfix operator '" + oper + "'", expr.oper);

    double value = std::get<double>(literal);
    double new_val = value;

    if (expr.oper.type == token_type::plus_plus_)
        new_val++;
    else if (expr.oper.type == token_type::minus_minus_)
        new_val--;

    _curr_env->assign(var_expr->ident_name.lexeme, new_val);
    return value;
}

literal_value interpreter::visit_prefix(prefix_expression& expr)
{
    // Unused for now.  This is currently being handled in visit_unary.
    assert(false);
}

literal_value interpreter::visit_call(call_expression& expr)
{
    literal_value literal = evaluate(expr.callee);
    geo_type type = literal_to_geo_type(literal);

    if (type != geo_type::callable_)
        throw geo_runtime_error("Can only call functions and classes '()'", expr.paren);

    geo_callable* function = std::get<geo_callable*>(literal);
    std::vector<literal_value> arguments;
    arguments.reserve(expr.arguments.size());

    for (std::unique_ptr<expression>& arg_expr : expr.arguments)
        arguments.emplace_back(evaluate(arg_expr));

    if (function->arity() != static_cast<int>(arguments.size()))
    {
        std::string msg = "Expected " + std::to_string(function->arity()) + " arguments but received " + std::to_string(arguments.size());
        throw geo_runtime_error(msg);
    }

    return function->call(*this, arguments);
}

bool interpreter::is_truthy(const literal_value& literal) const
{
    geo_type type = literal_to_geo_type(literal);

    switch (type)
    {
        case geo_type::bool_:
        {
            return std::get<bool>(literal);
        } break;
        case geo_type::number_:
        {
            return std::get<double>(literal) != 0;
        } break;
        case geo_type::string_:
        {
            return !std::get<std::string>(literal).empty();
        } break;
        case geo_type::null_:
        {
            return false;
        } break;
    }
    throw geo_runtime_error("Unknown type in is_truthy()");
}

bool interpreter::is_equal(const literal_value& lhs, const literal_value& rhs) const
{
    geo_type lhs_type = literal_to_geo_type(lhs);
    geo_type rhs_type = literal_to_geo_type(rhs);

    if (lhs_type != rhs_type)
    {
        return false;
    }

    if ((lhs_type == geo_type::null_) && (rhs_type == geo_type::null_))
        return true;
    if (lhs_type == geo_type::null_)
        return false;

    return lhs == rhs;
}

geo_type_error interpreter::type_error(const std::string& msg, const token& t) const
{
    return geo_type_error(msg, t);
}

NAMESPACE_END

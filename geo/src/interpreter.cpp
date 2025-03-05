#include "interpreter.h"
#include "console_io.h"
#include "debug_timer.h"
#include "environment.h"
#include "exceptions.h"
#include "expressions.h"
#include "geo_types.h"
#include "tokens.h"
#include "memory_manager.h"
#include "typedefs.h"
#include "statements.h"
#include <cassert>
#include <memory>
#include <string>
#include <vector>

NAMESPACE_BEGIN(geo)

interpreter::interpreter(console_io* io)
    : _env_manager()
    , _io(io) 
    , _locals()
{ 
    instantiate_standard_library();
}

void interpreter::instantiate_standard_library()
{
    memory_manager& instance = memory_manager::instance();

    geo_callable* clock = new class clock();
    geo_callable* print = new class print(_io);
    geo_callable* input = new class input(_io);
    _env_manager.get_global_environment()->define("clock", clock);
    _env_manager.get_global_environment()->define("print", print);
    _env_manager.get_global_environment()->define("input", input);
    instance.register_callable(clock);
    instance.register_callable(print);
    instance.register_callable(input);
}

void interpreter::interpret(const std::vector<std::unique_ptr<statement>>& statements)
{
#ifndef NDEBUG
    debug_timer dt("interpreter::interpret()");
#endif

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
    catch (...)
    {
        _io->err() << "Exception swallower hit\n";
    }

#ifndef NDEBUG
    dt.stop();
#endif
}

literal_value interpreter::evaluate(const std::unique_ptr<expression>& expr)
{
    return expr->accept_visitor(*this);
}

void interpreter::evaluate(const std::unique_ptr<statement>& stmt)
{
    stmt->accept_visitor(*this);
}

void interpreter::resolve(expression& expr, int depth)
{
    _locals[&expr] = depth;
}

literal_value interpreter::lookup_variable(const token& name, expression& expr)
{
    auto dist_it = _locals.find(&expr);
    if (dist_it != _locals.end())
    {
        return _env_manager.get_at(dist_it->second, name);
    }
    else
    {
        return _env_manager.get_global_environment()->get(name);
    }
}

void interpreter::visit_debug_statement(debug_statement& stmt)
{

}

void interpreter::visit_function_declaration_statement(function_declaration_statement& stmt)
{
    geo_callable* new_function = memory_manager::instance().allocate_user_function(stmt, _env_manager.get_current_environment(), &_env_manager);
    _env_manager.get_current_environment()->define(stmt.ident_name.lexeme, new_function);
}

void interpreter::visit_variable_declaration_statement(variable_declaration_statement& stmt)
{
    literal_value literal = undefined{};
    if (stmt.initializer_expr)
        literal = evaluate(stmt.initializer_expr);

    _env_manager.get_current_environment()->define(stmt.ident_name.lexeme, literal);
}

void interpreter::visit_if_statement(if_statement& stmt)
{
    if (is_truthy(evaluate(stmt.condition)))
    {
        evaluate(stmt.if_branch);
    }
    else
    {
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
        evaluate(stmt.stmt_body);
    }
}

void interpreter::visit_for_statement(for_statement& stmt)
{
    _env_manager.push_environment();

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

    _env_manager.pop_environment();
}

void interpreter::visit_break_statement(break_statement& stmt)
{
    throw geo_loop_break();
}

void interpreter::visit_continue_statement(continue_statement& stmt)
{
    throw geo_loop_continue();
}

void interpreter::visit_return_statement(return_statement& stmt)
{
    literal_value value = std::monostate{};

    if (stmt.return_expr)
        value = evaluate(stmt.return_expr);

    throw geo_function_return(value);
}

void interpreter::visit_block_statement(block_statement& stmt)
{
    _env_manager.push_environment();
    execute_block(stmt.statements, _env_manager.get_current_environment());
    _env_manager.pop_environment();
}

void interpreter::execute_block(const std::vector<std::unique_ptr<statement>>& statements, environment* new_environment)
{
    for (const auto& s : statements)
    {
        evaluate(s);
    }
}

void interpreter::visit_class_statement(class_statement& stmt)
{
    auto* curr_env = _env_manager.get_current_environment();
    curr_env->define(stmt.name.lexeme, std::monostate{});

    std::unordered_map<std::string, geo_callable*> methods;

    for (const std::unique_ptr<statement>& method : stmt.methods)
    {
        function_declaration_statement* casted_method = dynamic_cast<function_declaration_statement*>(method.get());
        if (!casted_method)
            throw geo_runtime_error("Class methods must be functions", stmt.name);

        geo_callable* new_method = memory_manager::instance().allocate_user_function(*casted_method, curr_env, &_env_manager);
        methods[casted_method->ident_name.lexeme] = new_method;
    }

    geo_callable* new_class = memory_manager::instance().allocate_class(stmt.name.lexeme, std::move(methods));
    curr_env->assign(stmt.name.lexeme, new_class);
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

        literal_value literal = _env_manager.get_current_environment()->get(var_expr->ident_name);
        geo_type type = literal_to_geo_type(literal);

        if (type != geo_type::number_)
            throw type_error("Invalid type for postfix operator '" + oper + "'", expr.oper);

        double value = std::get<double>(literal);

        if (expr.oper.type == token_type::plus_plus_)
            ++value;
        else if (expr.oper.type == token_type::minus_minus_)
            --value;

        _env_manager.get_current_environment()->assign(var_expr->ident_name.lexeme, value);
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
                geo_type_to_string(lhs_type) + " and " + geo_type_to_string(rhs_type));

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

literal_value interpreter::visit_literal(literal_expression& expr)
{
    return expr.literal_val;
}

literal_value interpreter::visit_grouping(grouping_expression& expr)
{
    return evaluate(expr.expr_group);
}

literal_value interpreter::visit_variable(variable_expression& expr)
{
    return lookup_variable(expr.ident_name, expr);
}

literal_value interpreter::visit_assignment(assignment_expression& expr)
{
    literal_value literal = evaluate(expr.initializer_expr);
    auto distance_it = _locals.find(&expr);

    if (distance_it != _locals.end())
    {
        _env_manager.assign_at(distance_it->second, expr.ident_name, literal);
    }
    else
    {
        _env_manager.get_global_environment()->assign(expr.ident_name.lexeme, literal);
    }

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

    literal_value literal = _env_manager.get_current_environment()->get(var_expr->ident_name);
    geo_type type = literal_to_geo_type(literal);

    if (type != geo_type::number_)
        throw type_error("Invalid type for postfix operator '" + oper + "'", expr.oper);

    double value = std::get<double>(literal);
    double new_val = value;

    if (expr.oper.type == token_type::plus_plus_)
        new_val++;
    else if (expr.oper.type == token_type::minus_minus_)
        new_val--;

    _env_manager.get_current_environment()->assign(var_expr->ident_name.lexeme, new_val);
    return value;
}

literal_value interpreter::visit_call(call_expression& expr)
{
    literal_value callee = expr.callee->accept_visitor(*this);

    std::vector<literal_value> args;
    args.reserve(expr.arguments.size());
    for (const auto& arg : expr.arguments)
        args.push_back(evaluate(arg));

    geo_type call_type = literal_to_geo_type(callee);
    if (call_type != geo_type::callable_)
        throw type_error("Cannot call '()' non-callable type", expr.paren);

    geo_callable* callable = std::get<geo_callable*>(callee);
    if (callable->arity() != static_cast<int>(args.size()))
        throw geo_runtime_error("Expected " + std::to_string(callable->arity()) + " arguments but got " + std::to_string(args.size()));

    return callable->call(*this, args);
}

literal_value interpreter::visit_get(get_expression& expr)
{
    literal_value object = evaluate(expr.object);
    geo_type object_type = literal_to_geo_type(object);

    if (object_type != geo_type::instance_)
        throw type_error("Only instances have properties", expr.name);

    geo_instance* instance = std::get<geo_instance*>(object);
    return instance->get(expr.name);
}

literal_value interpreter::visit_set(set_expression& expr)
{
    literal_value object = evaluate(expr.object);
    geo_type object_type = literal_to_geo_type(object);

    if (object_type != geo_type::instance_)
        throw type_error("Only instances have fields", expr.name);

    geo_instance* instance = std::get<geo_instance*>(object);
    literal_value value = evaluate(expr.value);
    instance->set(expr.name, value);
    return value;
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

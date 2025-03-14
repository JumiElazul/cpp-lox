#include "interpreter.h"
#include "console_io.h"
#include "debug_timer.h"
#include "environment.h"
#include "exceptions.h"
#include "expressions.h"
#include "cpplox_types.h"
#include "tokens.h"
#include "memory_manager.h"
#include "typedefs.h"
#include "statements.h"
#include <cassert>
#include <memory>
#include <string>
#include <vector>

NAMESPACE_BEGIN(cpplox)

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

    cpplox_callable* clock = new class clock();
    cpplox_callable* print = new class print(_io);
    cpplox_callable* input = new class input(_io);
    _env_manager.get_global_environment()->define("clock", clock);
    _env_manager.get_global_environment()->define("print", print);
    _env_manager.get_global_environment()->define("input", input);
    instance.register_callable(clock);
    instance.register_callable(print);
    instance.register_callable(input);
}

void interpreter::interpret(const std::vector<std::unique_ptr<statement>>& statements)
{
    debug_timer dt("interpreter::interpret()");

    try
    {
        for (const auto& stmt : statements)
        {
            evaluate(stmt);
        }
    }
    catch (const cpplox_runtime_error& e)
    {
        _io->err() << e.what() << '\n';
    }
    catch (...)
    {
        _io->err() << "Exception swallower hit\n";
    }

    dt.stop();
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
    cpplox_callable* new_function = memory_manager::instance().allocate_user_function(stmt, _env_manager.get_current_environment(), &_env_manager);
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
        catch (const cpplox_loop_break&)
        {
            break;
        }
        catch (const cpplox_loop_continue&)
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
    throw cpplox_loop_break();
}

void interpreter::visit_continue_statement(continue_statement& stmt)
{
    throw cpplox_loop_continue();
}

void interpreter::visit_return_statement(return_statement& stmt)
{
    literal_value value = std::monostate{};

    if (stmt.return_expr)
        value = evaluate(stmt.return_expr);

    throw cpplox_function_return(value);
}

void interpreter::visit_block_statement(block_statement& stmt)
{
    _env_manager.push_environment();
    try
    {
        execute_block(stmt.statements, _env_manager.get_current_environment());
    }
    catch (...)
    {
        // We need this here in the case of for_statements.  When continue/break statements are hit,
        // we need to cleanup the environment stack and rethrow it so the for_statement can catch it.
        _env_manager.pop_environment();
        throw;
    }
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
    cpplox_class* superclass = nullptr;

    if (stmt.superclass)
    {
        literal_value literal = evaluate(stmt.superclass);
        cpplox_type superclass_type = literal_to_cpplox_type(literal);

        if (superclass_type == cpplox_type::callable_)
        {
            cpplox_callable* callable = std::get<cpplox_callable*>(literal);
            cpplox_class* class_cast = dynamic_cast<cpplox_class*>(callable);

            if (!class_cast)
                throw cpplox_runtime_error("Could not superclass from attemped class, superclass specified was not a class", stmt.name);

            superclass = class_cast;
        }
    }

    _env_manager.get_current_environment()->define(stmt.name.lexeme, std::monostate{});

    if (stmt.superclass)
    {
        _env_manager.push_environment();
        _env_manager.get_current_environment()->define("super", superclass);
    }

    std::unordered_map<std::string, cpplox_callable*> methods;
    for (const std::unique_ptr<function_declaration_statement>& method : stmt.methods)
    {
        bool is_init = method->ident_name.lexeme == "init";
        cpplox_callable* new_method = memory_manager::instance().allocate_user_function(*method, _env_manager.get_current_environment(), &_env_manager, is_init);
        methods[method->ident_name.lexeme] = new_method;
    }

    cpplox_callable* new_class = memory_manager::instance().allocate_class(stmt.name.lexeme, std::move(methods), superclass);

    if (superclass)
        _env_manager.pop_environment();

    _env_manager.get_current_environment()->assign(stmt.name.lexeme, new_class);
}

void interpreter::visit_expression_statement(expression_statement& stmt)
{
    literal_value literal = evaluate(stmt.expr);
}

literal_value interpreter::visit_unary(unary_expression& expr)
{
    literal_value literal = evaluate(expr.expr_rhs);
    cpplox_type literal_type = literal_to_cpplox_type(literal);

    const token& oper = expr.oper;

    if (oper.type == token_type::bang_)
    {
        if (literal_type != cpplox_type::bool_)
            throw type_error("Cannot use unary operator ('!') on non-bool type", oper);

        return !is_truthy(literal);
    }

    if (oper.type == token_type::minus_)
    {
        if (literal_type != cpplox_type::number_)
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
        cpplox_type type = literal_to_cpplox_type(literal);

        if (type != cpplox_type::number_)
            throw type_error("Invalid type for postfix operator '" + oper + "'", expr.oper);

        double value = std::get<double>(literal);

        if (expr.oper.type == token_type::plus_plus_)
            ++value;
        else if (expr.oper.type == token_type::minus_minus_)
            --value;

        _env_manager.get_current_environment()->assign(var_expr->ident_name.lexeme, value);
        return value;
    }

    throw cpplox_type_error("Unknown unary operator", oper);
}

literal_value interpreter::visit_binary(binary_expression& expr)
{
    literal_value lhs = evaluate(expr.expr_lhs);
    literal_value rhs = evaluate(expr.expr_rhs);

    cpplox_type lhs_type = literal_to_cpplox_type(lhs);
    cpplox_type rhs_type = literal_to_cpplox_type(rhs);

    const token& oper = expr.oper;

    if (lhs_type != rhs_type)
    {
        if (oper.type == token_type::plus_)
        {
            if ((lhs_type == cpplox_type::string_ && rhs_type == cpplox_type::number_) ||
                    (lhs_type == cpplox_type::number_ && rhs_type == cpplox_type::string_))
            {
                if (lhs_type == cpplox_type::string_)
                {
                    return std::get<std::string>(lhs) + literal_value_to_runtime_string(rhs);
                }
                else
                {
                    return literal_value_to_runtime_string(lhs) + std::get<std::string>(rhs);
                }
            }
        }
        std::string msg = std::string("Cannot use binary operator '" + oper.lexeme + "' on types '" +
                cpplox_type_to_string(lhs_type) + "' and '" + cpplox_type_to_string(rhs_type)) + "'";
        throw cpplox_type_error(msg, oper);
    }

    switch (oper.type)
    {
        case token_type::plus_:
        {
            if (lhs_type == cpplox_type::string_)
            {
                return std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }
            else if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
            if (lhs_type == cpplox_type::number_)
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
    cpplox_type type = literal_to_cpplox_type(literal);

    if (type != cpplox_type::number_)
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

    cpplox_type call_type = literal_to_cpplox_type(callee);
    if (call_type != cpplox_type::callable_)
        throw type_error("Cannot call '()' non-callable type", expr.paren);

    cpplox_callable* callable = std::get<cpplox_callable*>(callee);
    if (callable->arity() != static_cast<int>(args.size()))
        throw cpplox_runtime_error("Expected " + std::to_string(callable->arity()) + " arguments but got " + std::to_string(args.size()));

    return callable->call(*this, args);
}

literal_value interpreter::visit_get(get_expression& expr)
{
    literal_value object = evaluate(expr.object);
    cpplox_type object_type = literal_to_cpplox_type(object);

    if (object_type == cpplox_type::instance_)
    {
        cpplox_instance* instance = std::get<cpplox_instance*>(object);
        return instance->get(expr.name);
    }
    else if (object_type == cpplox_type::callable_)
    {
        cpplox_callable* callable = std::get<cpplox_callable*>(object);
        cpplox_class* class_ = dynamic_cast<cpplox_class*>(callable);

        if (class_)
        {
            cpplox_callable* static_method = class_->find_method(expr.name);
            if (!static_method)
                throw cpplox_runtime_error("Static method with name '" + expr.name.lexeme + "' doesn't exist; are you trying to access an instance method or property?");

            return static_method;
        }
    }

    throw type_error("Only instances have properties", expr.name);
}

literal_value interpreter::visit_set(set_expression& expr)
{
    literal_value object = evaluate(expr.object);
    cpplox_type object_type = literal_to_cpplox_type(object);

    if (object_type != cpplox_type::instance_)
        throw type_error("Only instances have fields", expr.name);

    cpplox_instance* instance = std::get<cpplox_instance*>(object);
    literal_value value = evaluate(expr.value);
    instance->set(expr.name, value);
    return value;
}

literal_value interpreter::visit_this(this_expression& expr)
{
    return lookup_variable(expr.keyword, expr);
}

literal_value interpreter::visit_super(super_expression& expr)
{
    auto distance_it = _locals.find(&expr);

    if (distance_it == _locals.end())
        throw cpplox_runtime_error("Iterator distance_it in visit_super could not be resolved", expr.keyword);

    cpplox_class* superclass = std::get<cpplox_class*>(_env_manager.get_at(distance_it->second, create_dummy_token(token_type::super_)));

    if (!superclass)
        throw cpplox_runtime_error("Superclass could not be cast in visit_super", expr.keyword);

    cpplox_instance* object = std::get<cpplox_instance*>(_env_manager.get_at(distance_it->second - 1, create_dummy_token(token_type::this_)));
    if (!object)
        throw cpplox_runtime_error("Object could not be cast to a cpplox_instance* in visit_super", expr.keyword);

    cpplox_callable* method = superclass->find_method(expr.method.lexeme);
    if (!method)
        throw cpplox_runtime_error("Undefined property '" + expr.method.lexeme + "'.");

    user_function* method_cast = dynamic_cast<user_function*>(method);
    return method_cast->bind(object);
}

bool interpreter::is_truthy(const literal_value& literal) const
{
    cpplox_type type = literal_to_cpplox_type(literal);

    switch (type)
    {
        case cpplox_type::bool_:
        {
            return std::get<bool>(literal);
        } break;
        case cpplox_type::number_:
        {
            return std::get<double>(literal) != 0;
        } break;
        case cpplox_type::string_:
        {
            return !std::get<std::string>(literal).empty();
        } break;
        case cpplox_type::null_:
        {
            return false;
        } break;
    }
    throw cpplox_runtime_error("Unknown type in is_truthy()");
}

bool interpreter::is_equal(const literal_value& lhs, const literal_value& rhs) const
{
    cpplox_type lhs_type = literal_to_cpplox_type(lhs);
    cpplox_type rhs_type = literal_to_cpplox_type(rhs);

    if (lhs_type != rhs_type)
    {
        return false;
    }

    if ((lhs_type == cpplox_type::null_) && (rhs_type == cpplox_type::null_))
        return true;
    if (lhs_type == cpplox_type::null_)
        return false;

    return lhs == rhs;
}

cpplox_type_error interpreter::type_error(const std::string& msg, const token& t) const
{
    return cpplox_type_error(msg, t);
}

NAMESPACE_END

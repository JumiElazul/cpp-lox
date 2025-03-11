#include "resolver.h"
#include "console_io.h"
#include "debug_timer.h"
#include "exceptions.h"
#include "expressions.h"
#include "interpreter.h"
#include "statements.h"
#include "tokens.h"
#include "typedefs.h"
#include <cassert>
#include <memory>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(cpplox)

resolver::resolver(interpreter& interpreter_)
    : _interpreter(interpreter_)
    , _io(_interpreter._io)
    , _scopes()
    , _current_function_type(function_type::none)
    , _current_class_type(class_type::none_)
    , _had_error(false) { }

bool resolver::error_occurred() const noexcept { return _had_error; }
void resolver::reset_error_flag() noexcept { _had_error = false; }

void resolver::resolve_all(const std::vector<std::unique_ptr<statement>>& statements)
{
    debug_timer dt("resolver::resolve()");

    try
    {
        resolve(statements);
    }
    catch (const cpplox_runtime_error& e)
    {
        _io->err() << e.what() << '\n';
        _had_error = true;
    }

    dt.stop();
}

void resolver::resolve(const std::vector<std::unique_ptr<statement>>& statements)
{
    for (const auto& stmt : statements)
    {
        resolve(stmt);
    }
}

void resolver::resolve(const std::unique_ptr<statement>& stmt)
{
    stmt->accept_visitor(*this);
}

void resolver::resolve(const std::unique_ptr<expression>& expr)
{
    expr->accept_visitor(*this);
}

void resolver::visit_debug_statement(debug_statement& stmt)
{

}

void resolver::visit_function_declaration_statement(function_declaration_statement& stmt)
{
    declare(stmt.ident_name);
    define(stmt.ident_name);

    resolve_function(stmt, function_type::function);
}

void resolver::visit_variable_declaration_statement(variable_declaration_statement& stmt)
{
    declare(stmt.ident_name);
    if (stmt.initializer_expr)
        resolve(stmt.initializer_expr);
    define(stmt.ident_name);
}

void resolver::visit_if_statement(if_statement& stmt)
{
    resolve(stmt.condition);
    resolve(stmt.if_branch);

    if (stmt.else_branch)
        resolve(stmt.else_branch);
}

void resolver::visit_while_statement(while_statement& stmt)
{
    resolve(stmt.condition);
    resolve(stmt.stmt_body);
}

void resolver::visit_for_statement(for_statement& stmt)
{
    begin_scope();

    if (stmt.initializer)
        resolve(stmt.initializer);

    if (stmt.condition)
        resolve(stmt.condition);

    if (stmt.increment)
        resolve(stmt.increment);

    resolve(stmt.stmt_body);
    end_scope();
}

void resolver::visit_break_statement(break_statement& stmt)
{

}

void resolver::visit_continue_statement(continue_statement& stmt)
{

}

void resolver::visit_return_statement(return_statement& stmt)
{
    if (_current_function_type == function_type::none)
        throw cpplox_runtime_error("Invalid return found; return statement must be nested inside a function", stmt.keyword);

    if (stmt.return_expr)
    {
        if (_current_function_type == function_type::initializer)
            throw cpplox_runtime_error("Cannot return a value from an initializer", stmt.keyword);

        resolve(stmt.return_expr);
    }
}

void resolver::visit_block_statement(block_statement& stmt)
{
    begin_scope();
    resolve(stmt.statements);
    end_scope();
}

void resolver::visit_class_statement(class_statement& stmt)
{
    declare(stmt.name);
    define(stmt.name);

    if (stmt.superclass)
    {
        _current_class_type = class_type::subclass_;

        variable_expression* var_expr = dynamic_cast<variable_expression*>(stmt.superclass.get());
        if (var_expr && stmt.name.lexeme == var_expr->ident_name.lexeme)
            throw cpplox_runtime_error("A class cannot inherit from itself", stmt.name);

        resolve(stmt.superclass);
    }

    if (stmt.superclass)
    {
        begin_scope();
        _scopes.back()["super"] = variable_info{ true, true, create_dummy_token(token_type::super_) };
    }

    class_type enclosing_class = _current_class_type;
    _current_class_type = (stmt.superclass ? class_type::subclass_ : class_type::class_);

    begin_scope();

    token this_token = token{ token_type::this_, "this", "", { 0, 0 }, std::string("") };
    _scopes.back()["this"] = variable_info{ true, true, this_token };

    for (const std::unique_ptr<function_declaration_statement>& method : stmt.methods)
    {
        function_type declaration = method->static_method ? function_type::static_method : function_type::method;

        if (method->ident_name.lexeme == "init")
        {
            if (declaration == function_type::static_method)
                throw cpplox_runtime_error("Cannot use 'init' as a static method", method->ident_name);

            declaration = function_type::initializer;
        }

        resolve_function(*method, declaration);
    }

    if (stmt.superclass)
        end_scope();

    end_scope();

    _current_class_type = enclosing_class;
}

void resolver::visit_expression_statement(expression_statement& stmt)
{
    resolve(stmt.expr);
}

void resolver::visit_unary(unary_expression& expr)
{
    resolve(expr.expr_rhs);
}

void resolver::visit_binary(binary_expression& expr)
{
    resolve(expr.expr_lhs);
    resolve(expr.expr_rhs);
}

void resolver::visit_literal(literal_expression& expr)
{

}

void resolver::visit_grouping(grouping_expression& expr)
{
    resolve(expr.expr_group);
}

void resolver::visit_variable(variable_expression& expr)
{
    if (!_scopes.empty())
    {
        std::unordered_map<std::string, variable_info>& scope = _scopes.back();
        std::unordered_map<std::string, variable_info>::iterator it = scope.find(expr.ident_name.lexeme);

        if (it != scope.end())
        {
            if (it->second.defined == false)
                throw cpplox_runtime_error("Can't read local variable in its own initializer: " + it->first);

            it->second.used = true;
        }

        resolve_local(expr, expr.ident_name);
    }
}

void resolver::visit_assignment(assignment_expression& expr)
{
    resolve(expr.initializer_expr);
    resolve_local(expr, expr.ident_name);
}

void resolver::visit_logical(logical_expression& expr)
{
    resolve(expr.expr_lhs);
    resolve(expr.expr_rhs);
}

void resolver::visit_postfix(postfix_expression& expr)
{
    resolve(expr.expr_lhs);
}

void resolver::visit_call(call_expression& expr)
{
    resolve(expr.callee);

    for (const std::unique_ptr<expression>& arg : expr.arguments) 
    {
        resolve(arg);
    }
}

void resolver::visit_get(get_expression& expr)
{
    resolve(expr.object);
}

void resolver::visit_set(set_expression& expr)
{
    resolve(expr.object);
    resolve(expr.value);
}

void resolver::visit_this(this_expression& expr)
{
    if (_current_class_type == class_type::none_)
        throw cpplox_runtime_error("Cannot use 'this' outside of a class", expr.keyword);

    resolve_local(expr, expr.keyword);
}

void resolver::visit_super(super_expression& expr)
{
    if (_current_class_type == class_type::none_)
        throw cpplox_runtime_error("Can't use 'super' outside of a class.");
    else if (_current_class_type != class_type::subclass_)
        throw cpplox_runtime_error("Can't use 'super' in a class with no superclass.");

    resolve_local(expr, expr.keyword);
}

void resolver::begin_scope()
{
    _scopes.push_back(std::unordered_map<std::string, variable_info>());
}

void resolver::end_scope()
{
    if (!_scopes.empty())
    {
        auto& scope = _scopes.back();
        for (const auto& [name, info] : scope)
        {
            if (!info.used)
            {
                _io->err() << "Variable declared but never used: " << name << '\n';
            }
        }
    }

    _scopes.pop_back();
}

void resolver::declare(const token& t)
{
    if (_scopes.empty())
        return;

    auto& scope = _scopes.back();

    auto it = scope.find(t.lexeme);
    if (it != scope.end() && it->second.defined)
    {
        throw cpplox_runtime_error("Variable with this name already declared in this scope: " + t.lexeme);
    }

    scope[t.lexeme] = variable_info{ false, false, t };
}

void resolver::define(const token& t)
{
    if (_scopes.empty())
        return;

    auto& scope = _scopes.back();
    auto it = scope.find(t.lexeme);
    assert(it != scope.end());

    scope[t.lexeme].defined = true;
}

void resolver::resolve_local(expression& expr, const token& t)
{
    for (int i = static_cast<int>(_scopes.size()) - 1; i >= 0; --i)
    {
        auto& curr_scope = _scopes[static_cast<size_t>(i)];
        auto it = curr_scope.find(t.lexeme);
        if (it != curr_scope.end())
        {
            _interpreter.resolve(expr, static_cast<int>(static_cast<int>(_scopes.size()) - i - 1));
            return;
        }
    }
}

void resolver::resolve_function(function_declaration_statement& expr, function_type type)
{
    function_type enclosing_function = _current_function_type;
    _current_function_type = type;

    begin_scope();

    for (const token& t : expr.params)
    {
        declare(t);
        define(t);
    }

    resolve(expr.body);
    end_scope();

    _current_function_type = enclosing_function;
}

NAMESPACE_END


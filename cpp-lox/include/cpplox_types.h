#ifndef JUMI_CPPLOX_CPPLOX_TYPES_H
#define JUMI_CPPLOX_CPPLOX_TYPES_H
#include "typedefs.h"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

NAMESPACE_BEGIN(cpplox)

class console_io;
class function_declaration_statement;
class environment;
class environment_manager;
class cpplox_callable;
class cpplox_class;
class cpplox_instance;
class interpreter;

struct token;

enum class cpplox_type
{
    number_,
    string_,
    bool_,
    callable_,
    class_,
    instance_,
    null_,
    undefined_
};

struct undefined
{
    bool operator==(const undefined&) const { return true; }
    bool operator!=(const undefined&) const { return false; }
};

using literal_value = std::variant<
      double,
      bool,
      std::string,
      cpplox_callable*,
      cpplox_instance*,
      cpplox_class*,
      std::monostate,
      undefined>;

template<typename... Ts>
struct literal_value_overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
literal_value_overload(Ts...) -> literal_value_overload<Ts...>;

extern std::string cpplox_type_to_string(cpplox_type type);
extern cpplox_type literal_to_cpplox_type(const literal_value& l);
extern std::string literal_value_to_runtime_string(const literal_value& l);

class cpplox_callable
{
public:
    virtual ~cpplox_callable() = default;
    virtual int arity() = 0;
    virtual std::string to_string() const = 0;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) = 0;

protected:
    environment_manager* _env_manager;
};

class native_function : public cpplox_callable
{
public:
    virtual ~native_function() = default;
    virtual int arity() = 0;
    virtual std::string to_string() const = 0;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) = 0;
};

class user_function : public cpplox_callable
{
public:
    function_declaration_statement& declaration;
    environment* closure;

    user_function(function_declaration_statement& declaration_, environment* closure, environment_manager* env_manager, bool is_initializer = false);
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
    cpplox_callable* bind(cpplox_instance* instance);

protected:
    environment_manager* _env_manager;

private:
    bool _is_initializer;
};

class clock : public native_function
{
public:
    clock();
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
};

class print : public native_function
{
public:
    print(console_io* io);
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;

private:
    console_io* _io;
};

class input : public native_function
{
public:
    input(console_io* io);
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;

private:
    console_io* _io;
};

class cpplox_class : public cpplox_callable
{
public:
    std::string name;
    std::unordered_map<std::string, cpplox_callable*> methods;
    cpplox_class* superclass;

    cpplox_class(const std::string& name_, std::unordered_map<std::string, cpplox_callable*>&& methods_
            , cpplox_class* superclass_);

    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
    cpplox_callable* find_method(const token& name);
    cpplox_callable* find_method(const std::string& name);
};

class cpplox_instance
{
public:
    cpplox_instance(cpplox_class* class_);
    std::string to_string() const;
    literal_value get(const token& name);
    void set(const token& name, const literal_value& value);

private:
    cpplox_class* _class;
    std::unordered_map<std::string, literal_value> _fields;
};

NAMESPACE_END

#endif

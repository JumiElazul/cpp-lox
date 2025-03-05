#ifndef JUMI_GEO_GEO_TYPES_H
#define JUMI_GEO_GEO_TYPES_H
#include "typedefs.h"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

NAMESPACE_BEGIN(geo)

class console_io;
class function_declaration_statement;
class environment;
class environment_manager;
class geo_callable;
class geo_class;
class geo_instance;
class token;
class interpreter;

enum class geo_type
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

using literal_value = std::variant<double, bool, std::string, geo_callable*, geo_instance*, geo_class*, std::monostate, undefined>;

template<typename... Ts>
struct literal_value_overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
literal_value_overload(Ts...) -> literal_value_overload<Ts...>;

extern std::string geo_type_to_string(geo_type type);
extern geo_type literal_to_geo_type(const literal_value& l);
extern std::string literal_value_to_runtime_string(const literal_value& l);

class geo_callable
{
public:
    virtual ~geo_callable() = default;
    virtual int arity() = 0;
    virtual std::string to_string() const = 0;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) = 0;

protected:
    environment_manager* _env_manager;
};

class native_function : public geo_callable
{
public:
    virtual ~native_function() = default;
    virtual int arity() = 0;
    virtual std::string to_string() const = 0;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) = 0;
};

class user_function : public geo_callable
{
public:
    function_declaration_statement& declaration;
    environment* closure;

    user_function(function_declaration_statement& declaration_, environment* closure, environment_manager* env_manager);
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;

protected:
    environment_manager* _env_manager;
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

class geo_class : public geo_callable
{
public:
    std::string name;
    std::unordered_map<std::string, geo_callable*> methods;

    geo_class(const std::string& name_, std::unordered_map<std::string, geo_callable*>&& methods_);

    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
    geo_callable* find_method(const token& name);
};

class geo_instance
{
public:
    geo_instance(geo_class* class_);
    std::string to_string() const;
    literal_value get(const token& name) const;
    void set(const token& name, const literal_value& value);

private:
    geo_class* _class;
    std::unordered_map<std::string, literal_value> _fields;
};

NAMESPACE_END

#endif

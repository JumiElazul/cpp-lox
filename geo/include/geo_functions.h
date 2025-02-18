#ifndef JUMI_GEO_GEO_FUNCTIONS_H
#define JUMI_GEO_GEO_FUNCTIONS_H
#include "environment.h"
#include "typedefs.h"
#include "geo_types.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(geo)

class console_io;
class function_declaration_statement;
class interpreter;
class environment_manager;

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
    std::unique_ptr<function_declaration_statement> declaration;
    environment* closure;

    user_function(environment_manager* env_manager, std::unique_ptr<function_declaration_statement> declaration_, environment* closure);
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
    virtual ~clock() = default;
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
};

class print : public native_function
{
public:
    print(console_io* io);
    virtual ~print() = default;
    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;

private:
    console_io* _io;
};

NAMESPACE_END

#endif

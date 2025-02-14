#ifndef JUMI_GEO_GEO_FUNCTIONS_H
#define JUMI_GEO_GEO_FUNCTIONS_H
#include "statements.h"
#include "typedefs.h"
#include "geo_types.h"
#include "environment.h"
#include <vector>
#include <memory>

NAMESPACE_BEGIN(geo)

class interpreter;
class console_io;

class geo_callable
{
public:
    virtual ~geo_callable() = default;
    virtual int arity() = 0;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) = 0;
    virtual std::string to_string() const = 0;
};

class geo_function : public geo_callable
{
public:
    std::unique_ptr<function_declaration_statement> declaration;

    geo_function(std::unique_ptr<function_declaration_statement> declaration_);
    virtual ~geo_function() = default;
    virtual int arity() override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
    virtual std::string to_string() const override;
};

class clock : public geo_callable
{
public:
    virtual ~clock() = default;

    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
};

class print : public geo_callable
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

class input : public geo_callable
{
public:
    input(console_io* io);
    virtual ~input() = default;

    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;

private:
    console_io* _io;
};

class random : public geo_callable
{
public:
    virtual ~random() = default;

    virtual int arity() override;
    virtual std::string to_string() const override;
    virtual literal_value call(interpreter& i, const std::vector<literal_value>& args) override;
};

NAMESPACE_END

#endif

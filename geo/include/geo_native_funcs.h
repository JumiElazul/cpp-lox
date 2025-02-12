#ifndef JUMI_GEO_GEO_NATIVE_FUNCS_H
#define JUMI_GEO_GEO_NATIVE_FUNCS_H
#include "typedefs.h"
#include "geo_types.h"
#include <vector>

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

NAMESPACE_END

#endif

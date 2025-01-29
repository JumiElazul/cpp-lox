#ifndef JUMI_LUMINA_CONSOLE_IO_H
#define JUMI_LUMINA_CONSOLE_IO_H
#include "typedefs.h"
#include <iostream>
#include <initializer_list>
#include <string>

NAMESPACE_BEGIN(lumina)

struct token;
class console_io
{
public:
    console_io(std::ostream& os = std::cout);

    virtual void write(char c) const;
    virtual void write(const std::string& message) const;
    virtual void write(const std::initializer_list<std::string>& messages) const;
    virtual std::string readline() const;

    console_io& operator<<(const char c);
    console_io& operator<<(const std::string& message);
    console_io& operator<<(const token& t);

private:
    std::ostream& _os;
};

NAMESPACE_END

#endif

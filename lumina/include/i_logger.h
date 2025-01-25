#ifndef JUMI_LUMINA_I_LOGGER_H
#define JUMI_LUMINA_I_LOGGER_H
#include "typedefs.h"
#include <string>

NAMESPACE_BEGIN(lumina)

class i_logger
{
public:
    virtual ~i_logger() = default;

    virtual void log_trace(const std::string& message) const = 0;
    virtual void log_debug(const std::string& message) const = 0;
    virtual void log_info(const std::string& message) const = 0;
    virtual void log_warn(const std::string& message) const = 0;
    virtual void log_error(const std::string& message) const = 0;
    virtual void log_critical(const std::string& message) const = 0;
    virtual void print_log_levels() const = 0;
};

NAMESPACE_END

#endif


#ifndef JUMI_GEO_LOGGER_H
#define JUMI_GEO_LOGGER_H
#include "typedefs.h"
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

NAMESPACE_BEGIN(geo)

class logger
{
public:
    logger();

    void log_trace(const std::string& message) const;
    void log_debug(const std::string& message) const;
    void log_info(const std::string& message) const;
    void log_warn(const std::string& message) const;
    void log_error(const std::string& message) const;
    void log_critical(const std::string& message) const;
    void print_log_levels() const;

private:
    std::shared_ptr<spdlog::logger> _logger;
};

inline logger& get_global_logger()
{
    static logger instance;
    return instance;
}

#define GEO_TRACE(message) get_global_logger().log_trace(message)
#define GEO_DEBUG(message) get_global_logger().log_debug(message)
#define GEO_INFO(message) get_global_logger().log_info(message)
#define GEO_WARN(message) get_global_logger().log_warn(message)
#define GEO_ERROR(message) get_global_logger().log_error(message)
#define GEO_CRITICAL(message) get_global_logger().log_critical(message)

#define GEO_PRINT_LOG_LEVELS get_global_logger().print_log_levels()

NAMESPACE_END

#endif

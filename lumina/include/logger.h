#ifndef JUMI_LUMINA_LOGGER_H
#define JUMI_LUMINA_LOGGER_H
#include "typedefs.h"
#include "i_logger.h"
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

NAMESPACE_BEGIN(lumina)

class logger : public i_logger
{
public:
    logger();

    virtual void log_trace(const std::string& message) const override;
    virtual void log_debug(const std::string& message) const override;
    virtual void log_info(const std::string& message) const override;
    virtual void log_warn(const std::string& message) const override;
    virtual void log_error(const std::string& message) const override;
    virtual void log_critical(const std::string& message) const override;
    virtual void print_log_levels() const override;

private:
    std::shared_ptr<spdlog::logger> _logger;
};

inline i_logger& get_global_logger()
{
    static logger instance;
    return instance;
}

#define LUMINA_TRACE(message) get_global_logger().log_trace(message)
#define LUMINA_DEBUG(message) get_global_logger().log_debug(message)
#define LUMINA_INFO(message) get_global_logger().log_info(message)
#define LUMINA_WARN(message) get_global_logger().log_warn(message)
#define LUMINA_ERROR(message) get_global_logger().log_error(message)
#define LUMINA_CRITICAL(message) get_global_logger().log_critical(message)

#define LUMINA_PRINT_LOG_LEVELS get_global_logger().print_log_levels()

NAMESPACE_END

#endif

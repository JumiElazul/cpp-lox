#include "logger.h"
#include "typedefs.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <string>

NAMESPACE_BEGIN(cpplox)

logger::logger()
{
    std::vector<spdlog::sink_ptr> sinks;

// Only compile console loggers into debug builds
#if defined(CPPLOX_ENABLE_CONSOLE_LOGGING)
    #if !defined(NDEBUG)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(static_cast<spdlog::level::level_enum>(CPPLOX_CONSOLE_LOG_LEVEL));
        sinks.push_back(console_sink);
    #endif
#endif

#if defined(CPPLOX_ENABLE_FILE_LOGGING)
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("cpplox.log", false);
    file_sink->set_level(static_cast<spdlog::level::level_enum>(CPPLOX_FILE_LOG_LEVEL));
    sinks.push_back(file_sink);
#endif

    if (sinks.empty())
    {
        sinks.push_back(std::make_shared<spdlog::sinks::null_sink_mt>());
    }

    _logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
    _logger->set_level(static_cast<spdlog::level::level_enum>(spdlog::level::trace));
    _logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] [%s:%#] %v");

    spdlog::register_logger(_logger);
}

void logger::log_trace(const std::string& message) const
{
    SPDLOG_LOGGER_TRACE(_logger, message);
}

void logger::log_debug(const std::string& message) const
{
    SPDLOG_LOGGER_DEBUG(_logger, message);
}

void logger::log_info(const std::string& message) const
{
    SPDLOG_LOGGER_INFO(_logger, message);
}

void logger::log_warn(const std::string& message) const
{
    SPDLOG_LOGGER_WARN(_logger, message);
}

void logger::log_error(const std::string& message) const
{
    SPDLOG_LOGGER_ERROR(_logger, message);
}

void logger::log_critical(const std::string& message) const
{
    SPDLOG_LOGGER_CRITICAL(_logger, message);
}

void logger::print_log_levels() const
{
    log_trace("CPPLOX_TRACE level enabled");
    log_debug("CPPLOX_DEBUG level enabled");
    log_info("CPPLOX_INFO level enabled");
    log_warn("CPPLOX_WARN level enabled");
    log_error("CPPLOX_ERROR level enabled");
    log_critical("CPPLOX_CRITICAL level enabled");
}

NAMESPACE_END


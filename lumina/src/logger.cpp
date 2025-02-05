#include "logger.h"
#include "typedefs.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <string>

NAMESPACE_BEGIN(lumina)

#if defined(LUMINA_LOG_LEVEL_TRACE)
    #define DETERMINE_LOG_LEVEL _logger->set_level(spdlog::level::trace)
#elif defined(LUMINA_LOG_LEVEL_DEBUG)
    #define DETERMINE_LOG_LEVEL _logger->set_level(spdlog::level::debug)
#elif defined(LUMINA_LOG_LEVEL_INFO)
    #define DETERMINE_LOG_LEVEL _logger->set_level(spdlog::level::info)
#elif defined(LUMINA_LOG_LEVEL_WARN)
    #define DETERMINE_LOG_LEVEL _logger->set_level(spdlog::level::warn)
#elif defined(LUMINA_LOG_LEVEL_ERROR)
    #define DETERMINE_LOG_LEVEL _logger->set_level(spdlog::level::err)
#elif defined(LUMINA_LOG_LEVEL_CRITICAL)
    #define DETERMINE_LOG_LEVEL _logger->set_level(spdlog::level::critical)
#elif defined(LUMINA_LOG_LEVEL_OFF)
    #define DETERMINE_LOG_LEVEL
#endif

logger::logger()
{
    std::vector<spdlog::sink_ptr> sinks;

#if defined(LUMINA_ENABLE_CONSOLE_LOGGING)
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sinks.push_back(console_sink);
#endif

#if defined(LUMINA_ENABLE_FILE_LOGGING)
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("lumina.log", false);
    sinks.push_back(file_sink);
#endif

    if (sinks.empty())
    {
        sinks.push_back(std::make_shared<spdlog::sinks::null_sink_mt>());
    }

    _logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
    _logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] [%@] %v");
    DETERMINE_LOG_LEVEL;

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
    log_trace("LUMINA_TRACE level enabled");
    log_debug("LUMINA_DEBUG level enabled");
    log_info("LUMINA_INFO level enabled");
    log_warn("LUMINA_WARN level enabled");
    log_error("LUMINA_ERROR level enabled");
    log_critical("LUMINA_CRITICAL level enabled");
}

NAMESPACE_END


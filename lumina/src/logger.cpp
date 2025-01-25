#include "logger.h"
#include "typedefs.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
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
#endif

logger::logger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("lumina.log", false);
    std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };

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
    log_trace("LUMINA_TRACE Enabled");
    log_debug("LUMINA_DEBUG Enabled");
    log_info("LUMINA_INFO Enabled");
    log_warn("LUMINA_WARN Enabled");
    log_error("LUMINA_ERROR Enabled");
    log_critical("LUMINA_CRITICAL Enabled");
}

NAMESPACE_END


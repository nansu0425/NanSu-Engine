#include "EnginePCH.h"

namespace NanSu
{
    std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
    std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

    void Logger::Initialize()
    {
        // Log pattern: [Timestamp] [Logger Name] [Level] Message
        // %^ and %$ enable color based on log level
        spdlog::set_pattern("%^[%T] [%n] [%l] %v%$");

        // Create Engine logger (will use default colors based on log level)
        s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
        NS_ENGINE_ASSERT(s_EngineLogger != nullptr, "Failed to create Engine logger");
        s_EngineLogger->set_level(spdlog::level::trace);

        // Create Client logger (will use default colors based on log level)
        s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
        NS_ENGINE_ASSERT(s_ClientLogger != nullptr, "Failed to create Client logger");
        s_ClientLogger->set_level(spdlog::level::trace);

        NS_ENGINE_INFO("Logger initialized");
    }

    void Logger::Shutdown()
    {
        NS_ENGINE_INFO("Logger shutting down");

        s_EngineLogger.reset();
        s_ClientLogger.reset();

        spdlog::shutdown();
    }
}

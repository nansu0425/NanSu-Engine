#pragma once

#include <memory>
#include "spdlog/spdlog.h"

namespace NanSu
{
    class Logger
    {
    public:
        static void Initialize();
        static void Shutdown();

        static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Engine Log Macros
#define NS_ENGINE_TRACE(...)    ::NanSu::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define NS_ENGINE_INFO(...)     ::NanSu::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define NS_ENGINE_WARN(...)     ::NanSu::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define NS_ENGINE_ERROR(...)    ::NanSu::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define NS_ENGINE_CRITICAL(...) ::NanSu::Logger::GetEngineLogger()->critical(__VA_ARGS__)

// Client (Game/Editor) Log Macros
#define NS_TRACE(...)           ::NanSu::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define NS_INFO(...)            ::NanSu::Logger::GetClientLogger()->info(__VA_ARGS__)
#define NS_WARN(...)            ::NanSu::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define NS_ERROR(...)           ::NanSu::Logger::GetClientLogger()->error(__VA_ARGS__)
#define NS_CRITICAL(...)        ::NanSu::Logger::GetClientLogger()->critical(__VA_ARGS__)

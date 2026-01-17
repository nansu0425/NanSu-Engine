#include "EnginePCH.h"

#include "Core/Engine.h"
#include "Events/EventBus.h"
#include "Events/ApplicationEvent.h"

namespace NanSu
{
    Engine::Engine()
        : m_Version("1.0.0")
        , m_IsInitialized(false)
    {
        // Logger will be initialized in Initialize()
    }

    Engine::~Engine()
    {
        if (m_IsInitialized)
        {
            Shutdown();
        }
        // Logger has been shutdown in Shutdown()
    }

    void Engine::Initialize()
    {
        if (!m_IsInitialized)
        {
            Logger::Initialize();
            NS_ENGINE_INFO("Initializing NanSu Engine v{}", m_Version);

            EventBus::Initialize();

            // Publish initialization event
            AppInitEvent initEvent;
            EventBus::Publish(initEvent);

            m_IsInitialized = true;
        }
    }

    void Engine::Shutdown()
    {
        if (m_IsInitialized)
        {
            NS_ENGINE_INFO("Shutting down Engine");

            // Publish shutdown event
            AppShutdownEvent shutdownEvent;
            EventBus::Publish(shutdownEvent);

            EventBus::Shutdown();

            m_IsInitialized = false;
            Logger::Shutdown();
        }
    }

    std::string Engine::GetVersion() const
    {
        return m_Version;
    }

    void Engine::PrintInfo() const
    {
        NS_ENGINE_INFO("==================================");
        NS_ENGINE_INFO("    NanSu Engine");
        NS_ENGINE_INFO("    Version: {}", m_Version);

        std::string platform;
#ifdef NS_PLATFORM_WINDOWS
        platform = "Windows";
#else
        platform = "Unknown";
#endif
        NS_ENGINE_INFO("    Platform: {}", platform);

        std::string config;
#ifdef NS_DEBUG
        config = "Debug";
#elif defined(NS_RELEASE)
        config = "Release";
#elif defined(NS_DISTRIBUTION)
        config = "Distribution";
#endif
        NS_ENGINE_INFO("    Configuration: {}", config);
        NS_ENGINE_INFO("==================================");
    }
}

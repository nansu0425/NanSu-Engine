#include "Core/Engine.h"

namespace NanSu
{
    Engine::Engine()
        : m_Version("1.0.0")
        , m_IsInitialized(false)
    {
        std::cout << "[Engine] Constructor called" << std::endl;
    }

    Engine::~Engine()
    {
        if (m_IsInitialized)
        {
            Shutdown();
        }
        std::cout << "[Engine] Destructor called" << std::endl;
    }

    void Engine::Initialize()
    {
        if (!m_IsInitialized)
        {
            std::cout << "[Engine] Initializing NanSu Engine v" << m_Version << std::endl;
            m_IsInitialized = true;
        }
    }

    void Engine::Shutdown()
    {
        if (m_IsInitialized)
        {
            std::cout << "[Engine] Shutting down Engine" << std::endl;
            m_IsInitialized = false;
        }
    }

    std::string Engine::GetVersion() const
    {
        return m_Version;
    }

    void Engine::PrintInfo() const
    {
        std::cout << "==================================" << std::endl;
        std::cout << "    NanSu Engine" << std::endl;
        std::cout << "    Version: " << m_Version << std::endl;
        std::cout << "    Platform: ";
        
#ifdef NS_PLATFORM_WINDOWS
        std::cout << "Windows";
#else
        std::cout << "Unknown";
#endif

        std::cout << std::endl;
        std::cout << "    Configuration: ";

#ifdef NS_DEBUG
        std::cout << "Debug";
#elif defined(NS_RELEASE)
        std::cout << "Release";
#elif defined(NS_DISTRIBUTION)
        std::cout << "Distribution";
#endif

        std::cout << std::endl;
        std::cout << "==================================" << std::endl;
    }
}

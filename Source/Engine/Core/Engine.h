#pragma once

#include <string>
#include <iostream>

namespace NanSu
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Initialize();
        void Shutdown();
        
        std::string GetVersion() const;
        void PrintInfo() const;

    private:
        std::string m_Version;
        bool m_IsInitialized;
    };
}

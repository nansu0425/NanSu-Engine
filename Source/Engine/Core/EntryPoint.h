#pragma once

#include "Core/Application.h"
#include "Core/Logger.h"

#ifdef NS_PLATFORM_WINDOWS

/**
 * @brief Entry point for NanSu Engine applications
 *
 * This header provides the main() function for client applications.
 * Include this header in exactly one .cpp file per executable.
 *
 * The client must define:
 *   NanSu::Application* NanSu::CreateApplication();
 */

extern NanSu::Application* NanSu::CreateApplication();

int main(int argc, char** argv)
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    // Initialize core systems
    NanSu::Logger::Initialize();
    NS_ENGINE_INFO("=== NanSu Engine Starting ===");

    // Create and run the application
    NanSu::Application* app = NanSu::CreateApplication();
    app->Run();
    delete app;

    // Cleanup
    NS_ENGINE_INFO("=== NanSu Engine Shutdown Complete ===");
    NanSu::Logger::Shutdown();

    return 0;
}

#endif // NS_PLATFORM_WINDOWS

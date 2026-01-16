#include "Core/Engine.h"
#include "Core/Logger.h"
#include <iostream>

int main()
{
    NanSu::Engine engine;
    engine.Initialize();
    engine.PrintInfo();

    NS_INFO("========== Editor Application ==========");
    NS_INFO("Running editor-specific code...");
    NS_INFO("Editor is ready!");

    engine.Shutdown();

    std::cout << "\n[Editor] Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}

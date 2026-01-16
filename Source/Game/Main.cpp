#include "Core/Engine.h"
#include "Core/Logger.h"
#include <iostream>

int main()
{
    NanSu::Engine engine;
    engine.Initialize();
    engine.PrintInfo();

    NS_INFO("========== Game Application ==========");
    NS_INFO("Running game-specific code...");
    NS_INFO("Game is running!");

    engine.Shutdown();

    std::cout << "\n[Game] Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}

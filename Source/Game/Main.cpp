#include "Core/Engine.h"
#include <iostream>

int main()
{
    std::cout << "\n========== Game Application ==========" << std::endl;
    
    NanSu::Engine engine;
    engine.Initialize();
    engine.PrintInfo();
    
    std::cout << "\n[Game] Running game-specific code..." << std::endl;
    std::cout << "[Game] Game is running!" << std::endl;
    
    engine.Shutdown();
    
    std::cout << "\n[Game] Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}

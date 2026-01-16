#include "Core/Engine.h"
#include <iostream>

int main()
{
    std::cout << "\n========== Editor Application ==========" << std::endl;
    
    NanSu::Engine engine;
    engine.Initialize();
    engine.PrintInfo();
    
    std::cout << "\n[Editor] Running editor-specific code..." << std::endl;
    std::cout << "[Editor] Editor is ready!" << std::endl;
    
    engine.Shutdown();
    
    std::cout << "\n[Editor] Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}

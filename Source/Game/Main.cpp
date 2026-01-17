#include "Core/Application.h"
#include "Core/EntryPoint.h"

class GameApplication : public NanSu::Application
{
public:
    GameApplication()
    {
        NS_INFO("GameApplication created");
    }

    ~GameApplication()
    {
        NS_INFO("GameApplication destroyed");
    }
};

NanSu::Application* NanSu::CreateApplication()
{
    return new GameApplication();
}

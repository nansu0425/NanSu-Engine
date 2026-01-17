#include "Core/Application.h"
#include "Core/EntryPoint.h"

class EditorApplication : public NanSu::Application
{
public:
    EditorApplication()
    {
        NS_INFO("EditorApplication created");
    }

    ~EditorApplication()
    {
        NS_INFO("EditorApplication destroyed");
    }
};

NanSu::Application* NanSu::CreateApplication()
{
    return new EditorApplication();
}

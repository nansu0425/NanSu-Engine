#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Core/Layer.h"
#include <imgui.h>

class EditorLayer : public NanSu::Layer
{
public:
    EditorLayer() : Layer("EditorLayer") {}

    void OnImGuiRender() override
    {
        // ImGui Demo Window 표시
        static bool showDemo = true;
        if (showDemo)
            ImGui::ShowDemoWindow(&showDemo);
    }
};

class EditorApplication : public NanSu::Application
{
public:
    EditorApplication()
    {
        NS_INFO("EditorApplication created");
        PushLayer(new EditorLayer());
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

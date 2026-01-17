#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Input.h"
#include "Core/EntryPoint.h"

class ExampleLayer : public NanSu::Layer
{
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void OnAttach() override
    {
        NS_INFO("ExampleLayer::OnAttach");
    }

    void OnDetach() override
    {
        NS_INFO("ExampleLayer::OnDetach");
    }

    void OnUpdate() override
    {
        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::Space))
        {
            NS_INFO("Space key pressed!");
        }
    }

    void OnEvent(NanSu::Event& event) override
    {
        NS_TRACE("ExampleLayer: {}", event.ToString());
    }
};

class GameApplication : public NanSu::Application
{
public:
    GameApplication()
    {
        PushLayer(new ExampleLayer());
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

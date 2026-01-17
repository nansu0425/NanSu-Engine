#include "EnginePCH.h"
#include "Core/Application.h"
#include "Events/EventDispatcher.h"

namespace NanSu
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        NS_ENGINE_ASSERT(s_Instance == nullptr, "Application already exists!");
        s_Instance = this;

        // Create the main window
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(NS_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
        s_Instance = nullptr;
    }

    void Application::Run()
    {
        NS_ENGINE_INFO("Application starting main loop");

        while (m_Running)
        {
            // Process window messages
            m_Window->OnUpdate();

            // Skip update logic if minimized
            if (!m_Minimized)
            {
                // Future: Layer updates, rendering, etc.
            }
        }

        NS_ENGINE_INFO("Application exiting main loop");
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(NS_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(NS_BIND_EVENT_FN(Application::OnWindowResize));

        // Log events for debugging
        NS_ENGINE_TRACE("{}", event.ToString());
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        NS_ENGINE_INFO("WindowCloseEvent received - shutting down");
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        // Future: Notify renderer of resize
        return false;
    }
}

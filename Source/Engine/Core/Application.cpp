#include "EnginePCH.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "Events/EventDispatcher.h"
#include "ImGui/ImGuiLayer.h"

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

        // Initialize input system
        Input::Initialize();

        // Create and initialize graphics context
        m_GraphicsContext = std::unique_ptr<GraphicsContext>(
            GraphicsContext::Create(
                m_Window->GetNativeWindow(),
                m_Window->GetWidth(),
                m_Window->GetHeight()
            )
        );

        if (!m_GraphicsContext->Init())
        {
            NS_ENGINE_CRITICAL("Failed to initialize graphics context!");
        }

        // Create and push ImGui overlay
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        // Shutdown graphics context before window is destroyed
        if (m_GraphicsContext)
        {
            m_GraphicsContext->Shutdown();
        }

        Input::Shutdown();
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
                // Clear the screen with a dark blue color
                m_GraphicsContext->Clear(0.1f, 0.1f, 0.4f, 1.0f);

                // Update all layers (bottom to top)
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate();
                }

                // ImGui render pass
                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();

                // Present the frame
                m_GraphicsContext->SwapBuffers();
            }
        }

        NS_ENGINE_INFO("Application exiting main loop");
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(NS_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(NS_BIND_EVENT_FN(Application::OnWindowResize));

        // Propagate events to layers in reverse order (overlays first)
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (event.IsHandled())
                break;
            (*it)->OnEvent(event);
        }
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
        m_GraphicsContext->OnResize(event.GetWidth(), event.GetHeight());
        return false;
    }
}

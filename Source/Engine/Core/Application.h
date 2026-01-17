#pragma once

#include "Core/Types.h"
#include "Core/Window.h"
#include "Core/LayerStack.h"
#include "Renderer/GraphicsContext.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include <memory>

namespace NanSu
{
    /**
     * @brief Base application class
     *
     * Client applications derive from this class and implement
     * CreateApplication() to provide their custom application.
     * The Application owns the main window and runs the event loop.
     */
    class Application
    {
    public:
        Application();
        virtual ~Application();

        /**
         * @brief Main entry point - runs the application loop
         * Processes window messages and dispatches events until shutdown
         */
        void Run();

        /**
         * @brief Handle incoming events from the window
         * @param event The event to process
         */
        void OnEvent(Event& event);

        /**
         * @brief Push a layer onto the application's layer stack
         * @param layer The layer to push (Application takes ownership)
         */
        void PushLayer(Layer* layer);

        /**
         * @brief Push an overlay onto the application's layer stack
         * @param overlay The overlay to push (Application takes ownership)
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief Get the main window
         */
        Window& GetWindow() { return *m_Window; }

        /**
         * @brief Get the graphics context
         */
        GraphicsContext& GetGraphicsContext() { return *m_GraphicsContext; }

        /**
         * @brief Get the singleton application instance
         */
        static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

    private:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<GraphicsContext> m_GraphicsContext;
        LayerStack m_LayerStack;
        bool m_Running = true;
        bool m_Minimized = false;

        static Application* s_Instance;
    };

    /**
     * @brief Factory function to be defined by client application
     * @return Pointer to the client's Application instance
     *
     * Example:
     *   NanSu::Application* NanSu::CreateApplication()
     *   {
     *       return new GameApplication();
     *   }
     */
    Application* CreateApplication();
}

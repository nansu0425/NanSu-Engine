#pragma once

#include "Core/Types.h"
#include "Events/Event.h"
#include <string>
#include <functional>

namespace NanSu
{
    /**
     * @brief Window creation properties
     */
    struct WindowProps
    {
        std::string Title;
        uint32 Width;
        uint32 Height;

        WindowProps(const std::string& title = "NanSu Engine",
                    uint32 width = 1280,
                    uint32 height = 720)
            : Title(title)
            , Width(width)
            , Height(height)
        {
        }
    };

    /**
     * @brief Platform-independent window interface
     *
     * This is the abstract base class for all platform-specific window implementations.
     * Use Window::Create() factory method to create a platform-appropriate window.
     */
    class Window
    {
    public:
        using EventCallback = std::function<void(Event&)>;

        virtual ~Window() = default;

        /**
         * @brief Process pending window messages
         * Called once per frame to handle OS messages
         */
        virtual void OnUpdate() = 0;

        /**
         * @brief Get the window's client area width
         */
        virtual uint32 GetWidth() const = 0;

        /**
         * @brief Get the window's client area height
         */
        virtual uint32 GetHeight() const = 0;

        /**
         * @brief Set the callback function for window events
         * @param callback Function to be called when events occur
         */
        virtual void SetEventCallback(const EventCallback& callback) = 0;

        /**
         * @brief Get the native window handle
         * @return Platform-specific handle (HWND on Windows)
         */
        virtual void* GetNativeWindow() const = 0;

        /**
         * @brief Create a platform-specific window
         * @param props Window creation properties
         * @return Pointer to the created window (caller owns the memory)
         */
        static Window* Create(const WindowProps& props = WindowProps());
    };
}

#pragma once

#include "Core/Window.h"

#ifdef NS_PLATFORM_WINDOWS

namespace NanSu
{
    /**
     * @brief Win32 implementation of the Window interface
     *
     * Creates and manages a native Windows window using the Win32 API.
     * Converts Win32 messages to NanSu events.
     */
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        uint32 GetWidth() const override { return m_Data.Width; }
        uint32 GetHeight() const override { return m_Data.Height; }

        void SetEventCallback(const EventCallback& callback) override
        {
            m_Data.EventCallback = callback;
        }

        void* GetNativeWindow() const override { return m_Hwnd; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        /**
         * @brief Win32 window procedure callback
         * Converts Win32 messages to NanSu events
         */
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HWND m_Hwnd = nullptr;
        HINSTANCE m_HInstance = nullptr;

        /**
         * @brief Internal window data structure
         * Stored in GWLP_USERDATA for access from static WindowProc
         */
        struct WindowData
        {
            std::string Title;
            uint32 Width = 0;
            uint32 Height = 0;
            EventCallback EventCallback;
        };

        WindowData m_Data;

        static const wchar_t* s_ClassName;
        static bool s_ClassRegistered;
    };
}

#endif // NS_PLATFORM_WINDOWS

#include "EnginePCH.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Events/WindowEvent.h"

#ifdef NS_PLATFORM_WINDOWS

namespace NanSu
{
    // Static member definitions
    const wchar_t* WindowsWindow::s_ClassName = L"NanSuWindowClass";
    bool WindowsWindow::s_ClassRegistered = false;

    // Factory method implementation
    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        NS_ENGINE_INFO("Creating window: {} ({}x{})", props.Title, props.Width, props.Height);

        m_HInstance = GetModuleHandle(nullptr);

        // Register window class (only once)
        if (!s_ClassRegistered)
        {
            WNDCLASSEXW wc = {};
            wc.cbSize = sizeof(WNDCLASSEXW);
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc = WindowProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = m_HInstance;
            wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            wc.lpszMenuName = nullptr;
            wc.lpszClassName = s_ClassName;
            wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

            ATOM result = RegisterClassExW(&wc);
            NS_ENGINE_ASSERT(result != 0, "Failed to register window class");
            s_ClassRegistered = true;
        }

        // Calculate window size to achieve desired client area size
        DWORD style = WS_OVERLAPPEDWINDOW;
        RECT rect = { 0, 0, static_cast<LONG>(props.Width), static_cast<LONG>(props.Height) };
        AdjustWindowRect(&rect, style, FALSE);

        int32 windowWidth = rect.right - rect.left;
        int32 windowHeight = rect.bottom - rect.top;

        // Convert title to wide string for Unicode support
        int32 titleLen = MultiByteToWideChar(CP_UTF8, 0, props.Title.c_str(), -1, nullptr, 0);
        std::wstring wideTitle(static_cast<usize>(titleLen), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, props.Title.c_str(), -1, &wideTitle[0], titleLen);

        // Create the window
        m_Hwnd = CreateWindowExW(
            0,                              // Extended style
            s_ClassName,                    // Class name
            wideTitle.c_str(),              // Window title
            style,                          // Window style
            CW_USEDEFAULT, CW_USEDEFAULT,   // Position
            windowWidth, windowHeight,       // Size
            nullptr,                         // Parent window
            nullptr,                         // Menu
            m_HInstance,                     // Instance
            this                             // Additional data (passed to WM_NCCREATE)
        );

        NS_ENGINE_ASSERT(m_Hwnd != nullptr, "Failed to create window");

        ShowWindow(m_Hwnd, SW_SHOW);
        UpdateWindow(m_Hwnd);

        NS_ENGINE_INFO("Window created successfully");
    }

    void WindowsWindow::Shutdown()
    {
        if (m_Hwnd)
        {
            DestroyWindow(m_Hwnd);
            m_Hwnd = nullptr;
            NS_ENGINE_INFO("Window destroyed");
        }
    }

    void WindowsWindow::OnUpdate()
    {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // Handle WM_NCCREATE to store the WindowsWindow pointer
        if (msg == WM_NCCREATE)
        {
            CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            WindowsWindow* window = static_cast<WindowsWindow*>(createStruct->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&window->m_Data));
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        // Get WindowData pointer from GWLP_USERDATA
        WindowData* data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (data == nullptr || !data->EventCallback)
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        switch (msg)
        {
            case WM_CLOSE:
            {
                WindowCloseEvent event;
                data->EventCallback(event);
                return 0;  // Don't let DefWindowProc destroy the window
            }

            case WM_SIZE:
            {
                uint32 width = LOWORD(lParam);
                uint32 height = HIWORD(lParam);
                data->Width = width;
                data->Height = height;

                WindowResizeEvent event(width, height);
                data->EventCallback(event);
                return 0;
            }

            case WM_SETFOCUS:
            {
                WindowFocusEvent event;
                data->EventCallback(event);
                return 0;
            }

            case WM_KILLFOCUS:
            {
                WindowLostFocusEvent event;
                data->EventCallback(event);
                return 0;
            }

            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

#endif // NS_PLATFORM_WINDOWS

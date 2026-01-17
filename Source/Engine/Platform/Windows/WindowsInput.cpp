#include "EnginePCH.h"
#include "Platform/Windows/WindowsInput.h"
#include "Core/Application.h"

#ifdef NS_PLATFORM_WINDOWS

namespace NanSu
{
    // Factory method implementation - called from Input::Initialize()
    void Input::Initialize()
    {
        NS_ENGINE_ASSERT(!s_Instance, "Input system already initialized");
        s_Instance = std::make_unique<WindowsInput>();
        NS_ENGINE_INFO("Windows input system initialized");
    }

    bool WindowsInput::IsKeyPressedImpl(KeyCode key)
    {
        int32 vkCode = KeyCodeToVirtualKey(key);
        if (vkCode == 0)
        {
            return false;
        }

        SHORT state = GetAsyncKeyState(vkCode);
        // High bit indicates key is currently down
        return (state & 0x8000) != 0;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
    {
        int32 vkCode = 0;

        switch (button)
        {
            case MouseCode::Left:    vkCode = VK_LBUTTON;  break;
            case MouseCode::Right:   vkCode = VK_RBUTTON;  break;
            case MouseCode::Middle:  vkCode = VK_MBUTTON;  break;
            case MouseCode::Button3: vkCode = VK_XBUTTON1; break;
            case MouseCode::Button4: vkCode = VK_XBUTTON2; break;
            default: return false;
        }

        SHORT state = GetAsyncKeyState(vkCode);
        return (state & 0x8000) != 0;
    }

    std::pair<f32, f32> WindowsInput::GetMousePositionImpl()
    {
        POINT pt;
        GetCursorPos(&pt);

        // Convert to client coordinates if we have a window
        HWND hwnd = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
        if (hwnd)
        {
            ScreenToClient(hwnd, &pt);
        }

        return { static_cast<f32>(pt.x), static_cast<f32>(pt.y) };
    }
}

#endif // NS_PLATFORM_WINDOWS

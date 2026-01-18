#pragma once

#include "Core/Input.h"

#ifdef NS_PLATFORM_WINDOWS

namespace NanSu
{
    /**
     * @brief Win32 implementation of input polling
     *
     * Uses GetAsyncKeyState for keyboard polling and
     * GetCursorPos/ScreenToClient for mouse position.
     */
    class WindowsInput : public Input
    {
    public:
        WindowsInput() = default;
        virtual ~WindowsInput() = default;

    protected:
        bool IsKeyPressedImpl(KeyCode key) override;
        bool IsMouseButtonPressedImpl(MouseCode button) override;
        std::pair<float32, float32> GetMousePositionImpl() override;
    };
}

#endif // NS_PLATFORM_WINDOWS

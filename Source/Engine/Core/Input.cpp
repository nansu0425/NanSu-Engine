#include "EnginePCH.h"
#include "Core/Input.h"

namespace NanSu
{
    std::unique_ptr<Input> Input::s_Instance = nullptr;

    bool Input::IsKeyPressed(KeyCode key)
    {
        NS_ENGINE_ASSERT(s_Instance, "Input system not initialized");
        return s_Instance->IsKeyPressedImpl(key);
    }

    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        NS_ENGINE_ASSERT(s_Instance, "Input system not initialized");
        return s_Instance->IsMouseButtonPressedImpl(button);
    }

    std::pair<f32, f32> Input::GetMousePosition()
    {
        NS_ENGINE_ASSERT(s_Instance, "Input system not initialized");
        return s_Instance->GetMousePositionImpl();
    }

    f32 Input::GetMouseX()
    {
        return GetMousePosition().first;
    }

    f32 Input::GetMouseY()
    {
        return GetMousePosition().second;
    }

    void Input::Shutdown()
    {
        s_Instance.reset();
        NS_ENGINE_INFO("Input system shutdown");
    }
}

#pragma once

#include "Core/Types.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"
#include <utility>
#include <memory>

namespace NanSu
{
    /**
     * @brief Platform-independent input polling interface
     *
     * Provides static methods for polling keyboard and mouse state.
     * Platform-specific implementations are created via Input::Initialize().
     *
     * Usage:
     *   if (Input::IsKeyPressed(KeyCode::W))
     *       MoveForward();
     *
     *   auto [mouseX, mouseY] = Input::GetMousePosition();
     */
    class Input
    {
    public:
        virtual ~Input() = default;

        // Non-copyable
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        /**
         * @brief Check if a key is currently pressed
         * @param key The key code to check
         * @return true if the key is pressed, false otherwise
         */
        static bool IsKeyPressed(KeyCode key);

        /**
         * @brief Check if a mouse button is currently pressed
         * @param button The mouse button code to check
         * @return true if the button is pressed, false otherwise
         */
        static bool IsMouseButtonPressed(MouseCode button);

        /**
         * @brief Get the current mouse position in client coordinates
         * @return Pair of (x, y) coordinates
         */
        static std::pair<f32, f32> GetMousePosition();

        /**
         * @brief Get the current mouse X position in client coordinates
         * @return X coordinate
         */
        static f32 GetMouseX();

        /**
         * @brief Get the current mouse Y position in client coordinates
         * @return Y coordinate
         */
        static f32 GetMouseY();

        /**
         * @brief Initialize the input system
         * Creates platform-specific implementation
         */
        static void Initialize();

        /**
         * @brief Shutdown the input system
         */
        static void Shutdown();

    protected:
        Input() = default;

        // Platform-specific implementation methods
        virtual bool IsKeyPressedImpl(KeyCode key) = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual std::pair<f32, f32> GetMousePositionImpl() = 0;

    private:
        static std::unique_ptr<Input> s_Instance;
    };
}

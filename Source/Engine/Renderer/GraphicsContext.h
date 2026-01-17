#pragma once

#include "Core/Types.h"

namespace NanSu
{
    /**
     * @brief Platform-independent graphics context interface
     *
     * Abstract base class for all graphics API implementations (DX11, DX12, Vulkan).
     * Use GraphicsContext::Create() factory method to create an API-appropriate context.
     *
     * Responsibilities:
     * - Initialize graphics device and swap chain
     * - Clear the render target
     * - Present frames (swap buffers)
     * - Handle window resize
     */
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        // Non-copyable
        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;

        /**
         * @brief Initialize the graphics context
         * Creates device, device context, swap chain, and render target view
         * @return true if initialization succeeded
         */
        virtual bool Init() = 0;

        /**
         * @brief Shutdown and release all graphics resources
         */
        virtual void Shutdown() = 0;

        /**
         * @brief Clear the render target with a solid color
         * @param r Red component (0.0 - 1.0)
         * @param g Green component (0.0 - 1.0)
         * @param b Blue component (0.0 - 1.0)
         * @param a Alpha component (0.0 - 1.0)
         */
        virtual void Clear(f32 r, f32 g, f32 b, f32 a = 1.0f) = 0;

        /**
         * @brief Present the back buffer (swap buffers)
         * Should be called at the end of each frame
         */
        virtual void SwapBuffers() = 0;

        /**
         * @brief Handle window resize
         * Recreates swap chain buffers to match new dimensions
         * @param width New window width
         * @param height New window height
         */
        virtual void OnResize(uint32 width, uint32 height) = 0;

        /**
         * @brief Get the native device handle
         * @return Platform-specific device pointer (ID3D11Device* on DX11)
         */
        virtual void* GetNativeDevice() const = 0;

        /**
         * @brief Get the native device context handle
         * @return Platform-specific context pointer (ID3D11DeviceContext* on DX11)
         */
        virtual void* GetNativeDeviceContext() const = 0;

        /**
         * @brief Create a graphics context for the given window
         * @param windowHandle Native window handle (HWND on Windows)
         * @param width Window client width
         * @param height Window client height
         * @return Pointer to the created context (caller owns memory)
         */
        static GraphicsContext* Create(void* windowHandle, uint32 width, uint32 height);

    protected:
        GraphicsContext() = default;
    };
}

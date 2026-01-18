#pragma once

#include "Core/Types.h"

namespace NanSu
{
    // Forward declarations
    class VertexBuffer;
    class IndexBuffer;

    /**
     * @brief Primitive topology types for rendering
     */
    enum class PrimitiveTopology : uint8
    {
        None = 0,
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip
    };

    /**
     * @brief Abstract interface for low-level rendering API commands
     *
     * Platform-specific implementations (DX11RendererAPI, etc.) derive from this class.
     * Use RendererAPI::Create() factory method to create the appropriate implementation.
     *
     * This class is used internally by RenderCommand - application code should
     * use RenderCommand instead of accessing RendererAPI directly.
     */
    class RendererAPI
    {
    public:
        /**
         * @brief Supported graphics APIs
         */
        enum class API : uint8
        {
            None = 0,
            DirectX11,
            DirectX12,
            Vulkan
        };

    public:
        virtual ~RendererAPI() = default;

        // Non-copyable
        RendererAPI(const RendererAPI&) = delete;
        RendererAPI& operator=(const RendererAPI&) = delete;

        /**
         * @brief Initialize the renderer API
         */
        virtual void Init() = 0;

        /**
         * @brief Shutdown the renderer API
         */
        virtual void Shutdown() = 0;

        /**
         * @brief Set the viewport dimensions
         * @param x Viewport X position
         * @param y Viewport Y position
         * @param width Viewport width
         * @param height Viewport height
         */
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;

        /**
         * @brief Set the clear color for subsequent Clear() calls
         * @param r Red component (0.0 - 1.0)
         * @param g Green component (0.0 - 1.0)
         * @param b Blue component (0.0 - 1.0)
         * @param a Alpha component (0.0 - 1.0)
         */
        virtual void SetClearColor(float32 r, float32 g, float32 b, float32 a = 1.0f) = 0;

        /**
         * @brief Clear the currently bound render target
         */
        virtual void Clear() = 0;

        /**
         * @brief Set the primitive topology for subsequent draw calls
         * @param topology The primitive topology to use
         */
        virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;

        /**
         * @brief Bind the main render target for rendering
         */
        virtual void BindRenderTarget() = 0;

        /**
         * @brief Draw indexed geometry
         * @param indexBuffer The index buffer to use
         * @param indexCount Number of indices to draw (0 = use entire buffer)
         */
        virtual void DrawIndexed(const IndexBuffer* indexBuffer, uint32 indexCount = 0) = 0;

        /**
         * @brief Get the current graphics API
         * @return The active graphics API
         */
        static API GetAPI() { return s_API; }

        /**
         * @brief Create a RendererAPI instance for the current platform
         * @return Pointer to the created RendererAPI (caller owns memory)
         */
        static RendererAPI* Create();

    protected:
        RendererAPI() = default;

    private:
        static API s_API;
    };

}

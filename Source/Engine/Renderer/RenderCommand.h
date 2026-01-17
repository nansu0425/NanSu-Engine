#pragma once

#include "Renderer/RendererAPI.h"

namespace NanSu
{
    /**
     * @brief Static class providing immediate rendering commands
     *
     * RenderCommand is a static facade that delegates all calls to the
     * platform-specific RendererAPI implementation. This provides a clean,
     * easy-to-use API for low-level rendering operations.
     *
     * Example usage:
     * @code
     * RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
     * RenderCommand::Clear();
     * RenderCommand::DrawIndexed(indexBuffer);
     * @endcode
     */
    class RenderCommand
    {
    public:
        /**
         * @brief Initialize the render command system
         */
        static void Init()
        {
            s_RendererAPI = RendererAPI::Create();
            s_RendererAPI->Init();
        }

        /**
         * @brief Shutdown the render command system
         */
        static void Shutdown()
        {
            s_RendererAPI->Shutdown();
            delete s_RendererAPI;
            s_RendererAPI = nullptr;
        }

        /**
         * @brief Set the viewport dimensions
         */
        static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        /**
         * @brief Set the clear color for subsequent Clear() calls
         */
        static void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f)
        {
            s_RendererAPI->SetClearColor(r, g, b, a);
        }

        /**
         * @brief Clear the currently bound render target
         */
        static void Clear()
        {
            s_RendererAPI->Clear();
        }

        /**
         * @brief Set the primitive topology for subsequent draw calls
         */
        static void SetPrimitiveTopology(PrimitiveTopology topology)
        {
            s_RendererAPI->SetPrimitiveTopology(topology);
        }

        /**
         * @brief Bind the main render target
         */
        static void BindRenderTarget()
        {
            s_RendererAPI->BindRenderTarget();
        }

        /**
         * @brief Draw indexed geometry
         * @param indexBuffer The index buffer containing indices
         * @param indexCount Number of indices to draw (0 = entire buffer)
         */
        static void DrawIndexed(const IndexBuffer* indexBuffer, uint32 indexCount = 0)
        {
            s_RendererAPI->DrawIndexed(indexBuffer, indexCount);
        }

        /**
         * @brief Get the current graphics API
         */
        static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

    private:
        static RendererAPI* s_RendererAPI;
    };

}

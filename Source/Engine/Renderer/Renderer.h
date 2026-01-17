#pragma once

#include "Core/Types.h"
#include "Renderer/RenderCommand.h"

namespace NanSu
{
    // Forward declarations
    class Shader;
    class VertexBuffer;
    class IndexBuffer;

    /**
     * @brief High-level rendering orchestrator
     *
     * Provides scene-level rendering abstractions. Manages frame boundaries,
     * scene data (camera, lights), and executes draw calls.
     *
     * Example usage:
     * @code
     * Renderer::Init();
     *
     * // In game loop:
     * Renderer::BeginScene();
     * Renderer::Submit(shader, vertexBuffer, indexBuffer);
     * Renderer::EndScene();
     *
     * Renderer::Shutdown();
     * @endcode
     */
    class Renderer
    {
    public:
        /**
         * @brief Initialize the renderer system
         */
        static void Init();

        /**
         * @brief Shutdown the renderer system
         */
        static void Shutdown();

        /**
         * @brief Begin a new scene for rendering
         */
        static void BeginScene();

        /**
         * @brief End the current scene
         */
        static void EndScene();

        /**
         * @brief Submit a draw call for rendering
         * @param shader The shader to use for rendering
         * @param vertexBuffer The vertex buffer containing geometry
         * @param indexBuffer The index buffer containing indices
         */
        static void Submit(Shader* shader,
                           VertexBuffer* vertexBuffer,
                           IndexBuffer* indexBuffer);

        /**
         * @brief Handle window resize
         * @param width New window width
         * @param height New window height
         */
        static void OnWindowResize(uint32 width, uint32 height);

        /**
         * @brief Get the current graphics API
         */
        static RendererAPI::API GetAPI() { return RenderCommand::GetAPI(); }
    };

}

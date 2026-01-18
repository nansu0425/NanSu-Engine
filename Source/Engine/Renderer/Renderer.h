#pragma once

#include "Core/Types.h"
#include "Core/Math.h"
#include "Renderer/RenderCommand.h"

namespace NanSu
{
    // Forward declarations
    class Shader;
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;
    class OrthographicCamera;
    class Texture2D;

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
     * OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
     * Renderer::BeginScene(camera);
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
         * @brief Begin a new scene for rendering with the given camera
         * @param camera The camera providing view/projection matrices
         *
         * Uploads camera data to GPU constant buffer at slot b0
         */
        static void BeginScene(const OrthographicCamera& camera);

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
         * @brief Submit a textured draw call for rendering
         * @param shader The shader to use for rendering
         * @param vertexBuffer The vertex buffer containing geometry
         * @param indexBuffer The index buffer containing indices
         * @param texture The texture to bind (slot t0)
         */
        static void Submit(Shader* shader,
                           VertexBuffer* vertexBuffer,
                           IndexBuffer* indexBuffer,
                           Texture2D* texture);

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

    private:
        /**
         * @brief Scene data uploaded to GPU per-frame
         */
        struct SceneData
        {
            mat4 ViewProjectionMatrix;
        };

        static SceneData s_SceneData;
        static ConstantBuffer* s_SceneConstantBuffer;
        static Texture2D* s_WhiteTexture;  // 1x1 white fallback texture
    };

}

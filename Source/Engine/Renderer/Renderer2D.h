#pragma once

#include "Core/Types.h"
#include "Core/Math.h"

namespace NanSu
{
    // Forward declarations
    class OrthographicCamera;
    class Texture2D;

    /**
     * @brief High-level 2D rendering API
     *
     * Provides simple methods for drawing 2D primitives (quads).
     * Uses a single-shader strategy with white texture fallback for color-only rendering.
     *
     * Example usage:
     * @code
     * Renderer2D::Init();
     *
     * // In game loop:
     * Renderer2D::BeginScene(camera);
     * Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});  // Red quad
     * Renderer2D::DrawQuad({2.0f, 0.0f}, {1.0f, 1.0f}, texture);                    // Textured quad
     * Renderer2D::EndScene();
     *
     * Renderer2D::Shutdown();
     * @endcode
     */
    class Renderer2D
    {
    public:
        // Non-instantiable static class
        Renderer2D() = delete;

        // =====================================================================
        // Lifecycle
        // =====================================================================

        /**
         * @brief Initialize the 2D renderer
         * Creates shader, buffers, and white texture
         */
        static void Init();

        /**
         * @brief Shutdown the 2D renderer
         * Releases all resources
         */
        static void Shutdown();

        // =====================================================================
        // Scene Management
        // =====================================================================

        /**
         * @brief Begin a 2D rendering scene
         * @param camera The orthographic camera for this scene
         */
        static void BeginScene(const OrthographicCamera& camera);

        /**
         * @brief End the current scene
         * Future: Flushes batched draw calls
         */
        static void EndScene();

        // =====================================================================
        // Draw Primitives - Position + Size + Color
        // =====================================================================

        /**
         * @brief Draw a colored quad
         * @param position Center position (x, y)
         * @param size Width and height
         * @param color RGBA color (each component 0.0 - 1.0)
         */
        static void DrawQuad(const vec2& position, const vec2& size, const vec4& color);

        /**
         * @brief Draw a colored quad with z-depth
         * @param position Center position (x, y, z)
         * @param size Width and height
         * @param color RGBA color (each component 0.0 - 1.0)
         */
        static void DrawQuad(const vec3& position, const vec2& size, const vec4& color);

        // =====================================================================
        // Draw Primitives - Position + Size + Texture
        // =====================================================================

        /**
         * @brief Draw a textured quad (white tint = full texture color)
         * @param position Center position (x, y)
         * @param size Width and height
         * @param texture The texture to draw
         * @param tilingFactor UV tiling factor (default 1.0)
         */
        static void DrawQuad(const vec2& position, const vec2& size, Texture2D* texture,
                             float32 tilingFactor = 1.0f);

        /**
         * @brief Draw a textured quad with z-depth
         * @param position Center position (x, y, z)
         * @param size Width and height
         * @param texture The texture to draw
         * @param tilingFactor UV tiling factor (default 1.0)
         */
        static void DrawQuad(const vec3& position, const vec2& size, Texture2D* texture,
                             float32 tilingFactor = 1.0f);

        // =====================================================================
        // Draw Primitives - Position + Size + Texture + Tint
        // =====================================================================

        /**
         * @brief Draw a textured quad with tint color
         * @param position Center position (x, y)
         * @param size Width and height
         * @param texture The texture to draw
         * @param tintColor Color to multiply with texture (allows coloring)
         * @param tilingFactor UV tiling factor (default 1.0)
         */
        static void DrawQuad(const vec2& position, const vec2& size, Texture2D* texture,
                             const vec4& tintColor, float32 tilingFactor = 1.0f);

        /**
         * @brief Draw a textured quad with tint color and z-depth
         * @param position Center position (x, y, z)
         * @param size Width and height
         * @param texture The texture to draw
         * @param tintColor Color to multiply with texture (allows coloring)
         * @param tilingFactor UV tiling factor (default 1.0)
         */
        static void DrawQuad(const vec3& position, const vec2& size, Texture2D* texture,
                             const vec4& tintColor, float32 tilingFactor = 1.0f);

        // =====================================================================
        // Draw Primitives - Rotated + Color
        // =====================================================================

        /**
         * @brief Draw a rotated colored quad
         * @param position Center position (x, y)
         * @param size Width and height
         * @param rotation Rotation in radians (around Z axis)
         * @param color RGBA color
         */
        static void DrawRotatedQuad(const vec2& position, const vec2& size,
                                    float32 rotation, const vec4& color);

        /**
         * @brief Draw a rotated colored quad with z-depth
         * @param position Center position (x, y, z)
         * @param size Width and height
         * @param rotation Rotation in radians (around Z axis)
         * @param color RGBA color
         */
        static void DrawRotatedQuad(const vec3& position, const vec2& size,
                                    float32 rotation, const vec4& color);

        // =====================================================================
        // Draw Primitives - Rotated + Texture
        // =====================================================================

        /**
         * @brief Draw a rotated textured quad
         * @param position Center position (x, y)
         * @param size Width and height
         * @param rotation Rotation in radians (around Z axis)
         * @param texture The texture to draw
         * @param tilingFactor UV tiling factor
         * @param tintColor Color to multiply with texture
         */
        static void DrawRotatedQuad(const vec2& position, const vec2& size,
                                    float32 rotation, Texture2D* texture,
                                    float32 tilingFactor = 1.0f,
                                    const vec4& tintColor = vec4(1.0f));

        /**
         * @brief Draw a rotated textured quad with z-depth
         * @param position Center position (x, y, z)
         * @param size Width and height
         * @param rotation Rotation in radians (around Z axis)
         * @param texture The texture to draw
         * @param tilingFactor UV tiling factor
         * @param tintColor Color to multiply with texture
         */
        static void DrawRotatedQuad(const vec3& position, const vec2& size,
                                    float32 rotation, Texture2D* texture,
                                    float32 tilingFactor = 1.0f,
                                    const vec4& tintColor = vec4(1.0f));
    };

} // namespace NanSu

#pragma once

#include "Core/Types.h"
#include "Core/Math.h"

namespace NanSu
{
    /**
     * @brief 2D Orthographic camera for rendering 2D scenes
     *
     * The camera provides:
     * - View matrix: transforms world space to camera space
     * - Projection matrix: orthographic projection for 2D rendering
     * - ViewProjection matrix: combined for efficiency (single GPU upload)
     *
     * Coordinate system:
     * - Position (x, y): camera position in world space
     * - Rotation: rotation around Z-axis (degrees)
     *
     * Example usage:
     * @code
     * OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
     * camera.SetPosition({1.0f, 2.0f, 0.0f});
     * camera.SetRotation(45.0f);
     *
     * Renderer::BeginScene(camera);
     * Renderer::Submit(shader, vertexBuffer, indexBuffer);
     * Renderer::EndScene();
     * @endcode
     */
    class OrthographicCamera
    {
    public:
        /**
         * @brief Construct an orthographic camera with the given bounds
         * @param left Left boundary of the view
         * @param right Right boundary of the view
         * @param bottom Bottom boundary of the view
         * @param top Top boundary of the view
         *
         * Near/far planes default to -1.0f and 1.0f respectively
         */
        OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top);

        /**
         * @brief Set the projection bounds (useful for zoom/resize)
         * @param left Left boundary
         * @param right Right boundary
         * @param bottom Bottom boundary
         * @param top Top boundary
         */
        void SetProjection(float32 left, float32 right, float32 bottom, float32 top);

        /**
         * @brief Get the camera position in world space
         * @return Position as vec3 (z should be 0 for 2D)
         */
        const vec3& GetPosition() const { return m_Position; }

        /**
         * @brief Set the camera position in world space
         * @param position New position (z should be 0 for 2D)
         */
        void SetPosition(const vec3& position);

        /**
         * @brief Get the camera rotation around Z-axis
         * @return Rotation in degrees
         */
        float32 GetRotation() const { return m_Rotation; }

        /**
         * @brief Set the camera rotation around Z-axis
         * @param rotation Rotation in degrees
         */
        void SetRotation(float32 rotation);

        /**
         * @brief Get the projection matrix
         * @return Orthographic projection matrix
         */
        const mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

        /**
         * @brief Get the view matrix
         * @return View matrix (inverse of camera transform)
         */
        const mat4& GetViewMatrix() const { return m_ViewMatrix; }

        /**
         * @brief Get the combined view-projection matrix
         * @return ViewProjection matrix for shader upload
         */
        const mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        /**
         * @brief Recalculate the view matrix after position/rotation change
         */
        void RecalculateViewMatrix();

    private:
        mat4 m_ProjectionMatrix;
        mat4 m_ViewMatrix;
        mat4 m_ViewProjectionMatrix;

        vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float32 m_Rotation = 0.0f;  // Degrees, around Z-axis
    };

}

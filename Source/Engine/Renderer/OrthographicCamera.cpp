#include "EnginePCH.h"
#include "Renderer/OrthographicCamera.h"

namespace NanSu
{
    OrthographicCamera::OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
        , m_ViewMatrix(1.0f)  // Identity matrix
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float32 left, float32 right, float32 bottom, float32 top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetPosition(const vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetRotation(float32 rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        // Build transform matrix: T * R (translation * rotation)
        // Camera transform = where the camera is in world space
        mat4 transform = glm::translate(mat4(1.0f), m_Position)
                       * glm::rotate(mat4(1.0f), glm::radians(m_Rotation), vec3(0.0f, 0.0f, 1.0f));

        // View matrix is the INVERSE of the camera transform
        // This transforms world space to camera space
        m_ViewMatrix = glm::inverse(transform);

        // Recalculate combined matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}

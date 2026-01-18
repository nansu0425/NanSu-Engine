#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Core/Layer.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/OrthographicCamera.h"
#include <imgui.h>

class EditorLayer : public NanSu::Layer
{
public:
    EditorLayer()
        : Layer("EditorLayer")
        , m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)  // 16:9 aspect ratio
    {
    }

    void OnAttach() override
    {
        // Triangle vertex data (Position + Color)
        float vertices[] = {
            // Position (x, y, z)      Color (r, g, b, a)
            -0.5f, -0.5f, 0.0f,        1.0f, 0.0f, 0.0f, 1.0f,  // Bottom-left - Red
             0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f, 1.0f,  // Bottom-right - Green
             0.0f,  0.5f, 0.0f,        0.0f, 0.0f, 1.0f, 1.0f   // Top - Blue
        };

        // Index data (clockwise winding - D3D11 default front face)
        NanSu::uint32 indices[] = { 0, 2, 1 };

        // Create vertex buffer
        m_VertexBuffer = NanSu::VertexBuffer::Create(vertices, sizeof(vertices));
        m_VertexBuffer->SetLayout({
            { NanSu::ShaderDataType::Float3, "Position" },
            { NanSu::ShaderDataType::Float4, "Color" }
        });

        // Create index buffer
        m_IndexBuffer = NanSu::IndexBuffer::Create(indices, 3);

        // Create shader
        m_Shader = NanSu::Shader::Create("../../Assets/Shaders/Basic.hlsl");
        m_Shader->SetInputLayout(m_VertexBuffer->GetLayout());

        NS_INFO("EditorLayer: Triangle rendering with camera initialized");
    }

    void OnDetach() override
    {
        delete m_Shader;
        delete m_IndexBuffer;
        delete m_VertexBuffer;

        m_Shader = nullptr;
        m_IndexBuffer = nullptr;
        m_VertexBuffer = nullptr;
    }

    void OnUpdate() override
    {
        // Camera movement with keyboard input
        NanSu::float32 speed = 0.05f;

        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::A) ||
            NanSu::Input::IsKeyPressed(NanSu::KeyCode::Left))
        {
            m_CameraPosition.x -= speed;
        }
        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::D) ||
            NanSu::Input::IsKeyPressed(NanSu::KeyCode::Right))
        {
            m_CameraPosition.x += speed;
        }
        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::W) ||
            NanSu::Input::IsKeyPressed(NanSu::KeyCode::Up))
        {
            m_CameraPosition.y += speed;
        }
        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::S) ||
            NanSu::Input::IsKeyPressed(NanSu::KeyCode::Down))
        {
            m_CameraPosition.y -= speed;
        }

        // Camera rotation with Q/E
        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::Q))
        {
            m_CameraRotation += 1.0f;
        }
        if (NanSu::Input::IsKeyPressed(NanSu::KeyCode::E))
        {
            m_CameraRotation -= 1.0f;
        }

        // Update camera transform
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        // Render scene with camera
        NanSu::Renderer::BeginScene(m_Camera);
        NanSu::Renderer::Submit(m_Shader, m_VertexBuffer, m_IndexBuffer);
        NanSu::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        // Camera debug window
        ImGui::Begin("Camera Controls");

        ImGui::Text("Position: (%.2f, %.2f, %.2f)",
                    m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
        ImGui::Text("Rotation: %.2f degrees", m_CameraRotation);

        ImGui::Separator();
        ImGui::Text("Controls:");
        ImGui::BulletText("WASD / Arrows: Move camera");
        ImGui::BulletText("Q/E: Rotate camera");

        if (ImGui::Button("Reset Camera"))
        {
            m_CameraPosition = { 0.0f, 0.0f, 0.0f };
            m_CameraRotation = 0.0f;
        }

        ImGui::End();
    }

private:
    NanSu::OrthographicCamera m_Camera;
    NanSu::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    NanSu::float32 m_CameraRotation = 0.0f;

    NanSu::Shader* m_Shader = nullptr;
    NanSu::VertexBuffer* m_VertexBuffer = nullptr;
    NanSu::IndexBuffer* m_IndexBuffer = nullptr;
};

class EditorApplication : public NanSu::Application
{
public:
    EditorApplication()
    {
        NS_INFO("EditorApplication created");
        PushLayer(new EditorLayer());
    }

    ~EditorApplication()
    {
        NS_INFO("EditorApplication destroyed");
    }
};

NanSu::Application* NanSu::CreateApplication()
{
    return new EditorApplication();
}

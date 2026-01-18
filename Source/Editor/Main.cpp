#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Core/Layer.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/Texture.h"
#include "Renderer/OrthographicCamera.h"
#include <imgui.h>
#include <vector>
#include <string>

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
        // Quad vertex data (Position + Color + TexCoord)
        float vertices[] = {
            // Position (x, y, z)      Color (r, g, b, a)            TexCoord (u, v)
            -0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 1.0f, 1.0f,       0.0f, 0.0f,  // Bottom-left
             0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 1.0f, 1.0f,       1.0f, 0.0f,  // Bottom-right
             0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 1.0f, 1.0f,       1.0f, 1.0f,  // Top-right
            -0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 1.0f, 1.0f,       0.0f, 1.0f   // Top-left
        };

        // Index data for two triangles forming a quad (clockwise winding)
        NanSu::uint32 indices[] = {
            0, 2, 1,  // First triangle
            0, 3, 2   // Second triangle
        };

        // Create vertex buffer
        m_VertexBuffer = NanSu::VertexBuffer::Create(vertices, sizeof(vertices));
        m_VertexBuffer->SetLayout({
            { NanSu::ShaderDataType::Float3, "Position" },
            { NanSu::ShaderDataType::Float4, "Color" },
            { NanSu::ShaderDataType::Float2, "TexCoord" }
        });

        // Create index buffer
        m_IndexBuffer = NanSu::IndexBuffer::Create(indices, 6);

        // Create shader
        m_Shader = NanSu::Shader::Create("../../Assets/Shaders/Basic.hlsl");
        m_Shader->SetInputLayout(m_VertexBuffer->GetLayout());

        // Load all test textures
        LoadTextures();

        // Initialize Renderer2D
        NanSu::Renderer2D::Init();

        NS_INFO("EditorLayer: Textured quad rendering initialized");
    }

    void LoadTextures()
    {
        // List of textures to load
        const char* texturePaths[] = {
            "../../Assets/Textures/checkerboard.png",
            "../../Assets/Textures/gradient.png",
            "../../Assets/Textures/uv_test.png",
            "../../Assets/Textures/grid.png",
            "../../Assets/Textures/alpha_test.png"
        };

        const char* textureNames[] = {
            "Checkerboard",
            "Gradient",
            "UV Test",
            "Grid",
            "Alpha Test"
        };

        for (int i = 0; i < 5; ++i)
        {
            NanSu::Texture2D* texture = NanSu::Texture2D::Create(texturePaths[i]);
            m_Textures.push_back(texture);
            m_TextureNames.push_back(textureNames[i]);
        }

        m_CurrentTextureIndex = 0;
    }

    void OnDetach() override
    {
        // Shutdown Renderer2D
        NanSu::Renderer2D::Shutdown();

        // Delete all textures
        for (auto* texture : m_Textures)
        {
            delete texture;
        }
        m_Textures.clear();
        m_TextureNames.clear();

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

        // Update rotation for animated quad
        m_QuadRotation += 0.01f;

        // =========================================================================
        // Renderer2D Test
        // =========================================================================
        NanSu::Renderer2D::BeginScene(m_Camera);

        // Background quad (large, behind everything)
        NanSu::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 5.0f, 5.0f },
            { 0.2f, 0.2f, 0.3f, 1.0f });

        // Color-only quads (testing single shader strategy)
        NanSu::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f },
            { 1.0f, 0.0f, 0.0f, 1.0f });  // Red
        NanSu::Renderer2D::DrawQuad({ -0.5f, 0.0f }, { 0.5f, 0.5f },
            { 0.0f, 1.0f, 0.0f, 1.0f });  // Green
        NanSu::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f },
            { 0.0f, 0.0f, 1.0f, 1.0f });  // Blue

        // Textured quad
        NanSu::Texture2D* currentTexture = m_Textures.empty() ? nullptr : m_Textures[m_CurrentTextureIndex];
        if (currentTexture)
        {
            NanSu::Renderer2D::DrawQuad({ 0.8f, 0.0f }, { 0.8f, 0.8f }, currentTexture);
        }

        // Rotated quad (animated)
        NanSu::Renderer2D::DrawRotatedQuad({ 0.0f, 0.8f }, { 0.4f, 0.4f },
            m_QuadRotation, { 1.0f, 1.0f, 0.0f, 1.0f });  // Yellow, rotating

        // Textured quad with tint
        if (currentTexture)
        {
            NanSu::Renderer2D::DrawQuad({ -0.8f, 0.8f }, { 0.6f, 0.6f },
                currentTexture, { 1.0f, 0.5f, 0.5f, 1.0f });  // Red tint
        }

        NanSu::Renderer2D::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Controls");

        // Texture selection dropdown
        ImGui::Text("Texture Selection");
        if (ImGui::BeginCombo("Texture", m_TextureNames[m_CurrentTextureIndex].c_str()))
        {
            for (int i = 0; i < static_cast<int>(m_TextureNames.size()); ++i)
            {
                bool isSelected = (m_CurrentTextureIndex == i);
                if (ImGui::Selectable(m_TextureNames[i].c_str(), isSelected))
                {
                    m_CurrentTextureIndex = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Current texture info
        if (!m_Textures.empty() && m_Textures[m_CurrentTextureIndex])
        {
            NanSu::Texture2D* tex = m_Textures[m_CurrentTextureIndex];
            ImGui::Text("Size: %dx%d", tex->GetWidth(), tex->GetHeight());
        }

        ImGui::Separator();

        // Camera controls
        ImGui::Text("Camera");
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

    // Texture management
    std::vector<NanSu::Texture2D*> m_Textures;
    std::vector<std::string> m_TextureNames;
    int m_CurrentTextureIndex = 0;

    // Renderer2D test
    NanSu::float32 m_QuadRotation = 0.0f;
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

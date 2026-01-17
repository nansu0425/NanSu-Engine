#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Core/Layer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include <imgui.h>

class EditorLayer : public NanSu::Layer
{
public:
    EditorLayer() : Layer("EditorLayer") {}

    void OnAttach() override
    {
        // 삼각형 버텍스 데이터 (Position + Color)
        float vertices[] = {
            // Position (x, y, z)      Color (r, g, b, a)
            -0.5f, -0.5f, 0.0f,        1.0f, 0.0f, 0.0f, 1.0f,  // 좌하단 - 빨강
             0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f, 1.0f,  // 우하단 - 초록
             0.0f,  0.5f, 0.0f,        0.0f, 0.0f, 1.0f, 1.0f   // 상단 - 파랑
        };

        // 인덱스 데이터 (시계방향 와인딩 - D3D11 기본 앞면)
        NanSu::uint32 indices[] = { 0, 2, 1 };

        // 버텍스 버퍼 생성
        m_VertexBuffer = NanSu::VertexBuffer::Create(vertices, sizeof(vertices));
        m_VertexBuffer->SetLayout({
            { NanSu::ShaderDataType::Float3, "Position" },
            { NanSu::ShaderDataType::Float4, "Color" }
        });

        // 인덱스 버퍼 생성
        m_IndexBuffer = NanSu::IndexBuffer::Create(indices, 3);

        // 셰이더 생성 (VS 작업 디렉토리 기준: Source/Editor/ → 프로젝트 루트)
        m_Shader = NanSu::Shader::Create("../../Assets/Shaders/Basic.hlsl");
        m_Shader->SetInputLayout(m_VertexBuffer->GetLayout());

        NS_INFO("EditorLayer: Triangle rendering initialized");
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
        NanSu::Renderer::Submit(m_Shader, m_VertexBuffer, m_IndexBuffer);
    }

    void OnImGuiRender() override
    {
        // ImGui Demo Window 표시
        static bool showDemo = true;
        if (showDemo)
            ImGui::ShowDemoWindow(&showDemo);
    }

private:
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

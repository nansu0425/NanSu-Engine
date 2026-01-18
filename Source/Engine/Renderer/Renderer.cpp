#include "EnginePCH.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/OrthographicCamera.h"

namespace NanSu
{
    // Static member definitions
    Renderer::SceneData Renderer::s_SceneData;
    ConstantBuffer* Renderer::s_SceneConstantBuffer = nullptr;

    void Renderer::Init()
    {
        NS_ENGINE_INFO("Initializing Renderer");
        RenderCommand::Init();

        // Create scene constant buffer for camera data (slot b0)
        s_SceneConstantBuffer = ConstantBuffer::Create(sizeof(SceneData));

        NS_ENGINE_INFO("Renderer initialized with scene constant buffer");
    }

    void Renderer::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down Renderer");

        delete s_SceneConstantBuffer;
        s_SceneConstantBuffer = nullptr;

        RenderCommand::Shutdown();
        NS_ENGINE_INFO("Renderer shut down");
    }

    void Renderer::BeginScene(const OrthographicCamera& camera)
    {
        // Store scene data (transpose for HLSL row-major layout)
        s_SceneData.ViewProjectionMatrix = glm::transpose(camera.GetViewProjectionMatrix());

        // Upload to GPU
        s_SceneConstantBuffer->SetData(&s_SceneData, sizeof(SceneData));
        s_SceneConstantBuffer->Bind(0);  // Bind to slot b0
    }

    void Renderer::EndScene()
    {
        // Future: Flush batched draw calls if implementing batching
    }

    void Renderer::Submit(Shader* shader,
                          VertexBuffer* vertexBuffer,
                          IndexBuffer* indexBuffer)
    {
        shader->Bind();
        vertexBuffer->Bind();
        indexBuffer->Bind();
        RenderCommand::DrawIndexed(indexBuffer);
    }

    void Renderer::OnWindowResize(uint32 width, uint32 height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

}

#include "EnginePCH.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

namespace NanSu
{
    void Renderer::Init()
    {
        NS_ENGINE_INFO("Initializing Renderer");
        RenderCommand::Init();
        NS_ENGINE_INFO("Renderer initialized");
    }

    void Renderer::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down Renderer");
        RenderCommand::Shutdown();
        NS_ENGINE_INFO("Renderer shut down");
    }

    void Renderer::BeginScene()
    {
        // Future: Accept camera/scene data and upload to GPU
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

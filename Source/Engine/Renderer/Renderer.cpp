#include "EnginePCH.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/OrthographicCamera.h"

namespace NanSu
{
    // Static member definitions
    Renderer::SceneData Renderer::s_SceneData;
    ConstantBuffer* Renderer::s_SceneConstantBuffer = nullptr;
    Texture2D* Renderer::s_WhiteTexture = nullptr;

    void Renderer::Init()
    {
        NS_ENGINE_INFO("Initializing Renderer");
        RenderCommand::Init();

        // Create scene constant buffer for camera data (slot b0)
        s_SceneConstantBuffer = ConstantBuffer::Create(sizeof(SceneData));

        // Create 1x1 white fallback texture
        s_WhiteTexture = Texture2D::Create(1, 1);
        uint32 whitePixel = 0xFFFFFFFF;  // RGBA: white, fully opaque
        s_WhiteTexture->SetData(&whitePixel, sizeof(uint32));

        NS_ENGINE_INFO("Renderer initialized");
    }

    void Renderer::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down Renderer");

        delete s_WhiteTexture;
        s_WhiteTexture = nullptr;

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
        s_WhiteTexture->Bind(0);  // Use white fallback texture
        vertexBuffer->Bind();
        indexBuffer->Bind();
        RenderCommand::DrawIndexed(indexBuffer);
    }

    void Renderer::Submit(Shader* shader,
                          VertexBuffer* vertexBuffer,
                          IndexBuffer* indexBuffer,
                          Texture2D* texture)
    {
        shader->Bind();
        if (texture)
        {
            texture->Bind(0);  // Bind user texture to slot t0
        }
        else
        {
            s_WhiteTexture->Bind(0);  // Use white fallback texture
        }
        vertexBuffer->Bind();
        indexBuffer->Bind();
        RenderCommand::DrawIndexed(indexBuffer);
    }

    void Renderer::OnWindowResize(uint32 width, uint32 height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

}

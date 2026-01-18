#include "EnginePCH.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/OrthographicCamera.h"

namespace NanSu
{
    // =========================================================================
    // Internal Data Structure
    // =========================================================================

    struct Renderer2DData
    {
        // Scene data (uploaded to GPU per-frame)
        struct SceneData
        {
            mat4 ViewProjectionMatrix;
        };

        // Quad vertex structure (44 bytes per vertex)
        struct QuadVertex
        {
            vec3 Position;          // 12 bytes
            vec4 Color;             // 16 bytes
            vec2 TexCoord;          // 8 bytes
            float32 TexIndex;       // 4 bytes (for future batching)
            float32 TilingFactor;   // 4 bytes
        };

        // GPU Resources
        Shader* QuadShader = nullptr;
        VertexBuffer* QuadVertexBuffer = nullptr;   // Dynamic buffer
        IndexBuffer* QuadIndexBuffer = nullptr;     // Static buffer
        ConstantBuffer* SceneConstantBuffer = nullptr;
        Texture2D* WhiteTexture = nullptr;

        // CPU vertex data for immediate mode (single quad = 4 vertices)
        QuadVertex QuadVertexData[4];

        // Base quad vertex positions (centered at origin, unit size)
        // Used for transform calculations
        vec4 QuadVertexPositions[4];

        // Scene state
        SceneData CurrentSceneData;
    };

    // Static data instance
    static Renderer2DData s_Data;

    // =========================================================================
    // Lifecycle
    // =========================================================================

    void Renderer2D::Init()
    {
        NS_ENGINE_INFO("Initializing Renderer2D");

        // Initialize base quad vertex positions (centered at origin, unit size)
        // These will be transformed by model matrix for each quad
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };  // Bottom-left
        s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };  // Bottom-right
        s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };  // Top-right
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };  // Top-left

        // Create shader (path relative to executable in Binaries/{Config}/Editor/)
        s_Data.QuadShader = Shader::Create("../../Assets/Shaders/Renderer2D.hlsl");

        // Create dynamic vertex buffer (4 vertices per quad)
        s_Data.QuadVertexBuffer = VertexBuffer::CreateDynamic(
            sizeof(Renderer2DData::QuadVertex) * 4);

        // Set buffer layout
        BufferLayout layout = {
            { ShaderDataType::Float3, "Position" },
            { ShaderDataType::Float4, "Color" },
            { ShaderDataType::Float2, "TexCoord" },
            { ShaderDataType::Float,  "TexIndex" },
            { ShaderDataType::Float,  "TilingFactor" }
        };
        s_Data.QuadVertexBuffer->SetLayout(layout);
        s_Data.QuadShader->SetInputLayout(layout);

        // Create index buffer (static pattern for a single quad - 2 triangles)
        uint32 indices[] = { 0, 2, 1, 0, 3, 2 };
        s_Data.QuadIndexBuffer = IndexBuffer::Create(indices, 6);

        // Create scene constant buffer (slot b0)
        s_Data.SceneConstantBuffer = ConstantBuffer::Create(
            sizeof(Renderer2DData::SceneData));

        // Create 1x1 white texture for color-only rendering
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32 whitePixel = 0xFFFFFFFF;  // RGBA: white, fully opaque
        s_Data.WhiteTexture->SetData(&whitePixel, sizeof(uint32));

        NS_ENGINE_INFO("Renderer2D initialized");
    }

    void Renderer2D::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down Renderer2D");

        delete s_Data.WhiteTexture;
        s_Data.WhiteTexture = nullptr;

        delete s_Data.SceneConstantBuffer;
        s_Data.SceneConstantBuffer = nullptr;

        delete s_Data.QuadIndexBuffer;
        s_Data.QuadIndexBuffer = nullptr;

        delete s_Data.QuadVertexBuffer;
        s_Data.QuadVertexBuffer = nullptr;

        delete s_Data.QuadShader;
        s_Data.QuadShader = nullptr;

        NS_ENGINE_INFO("Renderer2D shut down");
    }

    // =========================================================================
    // Scene Management
    // =========================================================================

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        // Store scene data (transpose for HLSL row-major layout)
        s_Data.CurrentSceneData.ViewProjectionMatrix =
            glm::transpose(camera.GetViewProjectionMatrix());

        // Upload to GPU
        s_Data.SceneConstantBuffer->SetData(&s_Data.CurrentSceneData,
            sizeof(Renderer2DData::SceneData));
        s_Data.SceneConstantBuffer->Bind(0);  // Bind to slot b0
    }

    void Renderer2D::EndScene()
    {
        // Future: Flush batched draw calls if implementing batching
    }

    // =========================================================================
    // Internal Draw Implementation
    // =========================================================================

    namespace
    {
        // Texture coordinates for a quad
        constexpr vec2 s_TexCoords[] = {
            { 0.0f, 1.0f },  // Bottom-left (UV: 0,1 for DX texture coordinates)
            { 1.0f, 1.0f },  // Bottom-right
            { 1.0f, 0.0f },  // Top-right
            { 0.0f, 0.0f }   // Top-left
        };

        void DrawQuadInternal(const vec3& position, const vec2& size,
                              Texture2D* texture, const vec4& color,
                              float32 tilingFactor)
        {
            // Calculate transform matrix (translation + scale, no rotation)
            mat4 transform = glm::translate(mat4(1.0f), position)
                           * glm::scale(mat4(1.0f), { size.x, size.y, 1.0f });

            // Set up vertex data
            for (uint32 i = 0; i < 4; i++)
            {
                vec4 worldPos = transform * s_Data.QuadVertexPositions[i];
                s_Data.QuadVertexData[i].Position = vec3(worldPos);
                s_Data.QuadVertexData[i].Color = color;
                s_Data.QuadVertexData[i].TexCoord = s_TexCoords[i];
                s_Data.QuadVertexData[i].TexIndex = 0.0f;  // Single texture for now
                s_Data.QuadVertexData[i].TilingFactor = tilingFactor;
            }

            // Upload vertex data to GPU
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexData,
                sizeof(Renderer2DData::QuadVertex) * 4);

            // Bind resources
            s_Data.QuadShader->Bind();

            if (texture)
            {
                texture->Bind(0);
            }
            else
            {
                s_Data.WhiteTexture->Bind(0);
            }

            s_Data.QuadVertexBuffer->Bind();
            s_Data.QuadIndexBuffer->Bind();

            // Draw
            RenderCommand::DrawIndexed(s_Data.QuadIndexBuffer, 6);
        }

        void DrawRotatedQuadInternal(const vec3& position, const vec2& size,
                                     float32 rotation, Texture2D* texture,
                                     const vec4& color, float32 tilingFactor)
        {
            // Calculate transform matrix with rotation (TRS order)
            mat4 transform = glm::translate(mat4(1.0f), position)
                           * glm::rotate(mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
                           * glm::scale(mat4(1.0f), { size.x, size.y, 1.0f });

            // Set up vertex data
            for (uint32 i = 0; i < 4; i++)
            {
                vec4 worldPos = transform * s_Data.QuadVertexPositions[i];
                s_Data.QuadVertexData[i].Position = vec3(worldPos);
                s_Data.QuadVertexData[i].Color = color;
                s_Data.QuadVertexData[i].TexCoord = s_TexCoords[i];
                s_Data.QuadVertexData[i].TexIndex = 0.0f;
                s_Data.QuadVertexData[i].TilingFactor = tilingFactor;
            }

            // Upload vertex data to GPU
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexData,
                sizeof(Renderer2DData::QuadVertex) * 4);

            // Bind resources
            s_Data.QuadShader->Bind();

            if (texture)
            {
                texture->Bind(0);
            }
            else
            {
                s_Data.WhiteTexture->Bind(0);
            }

            s_Data.QuadVertexBuffer->Bind();
            s_Data.QuadIndexBuffer->Bind();

            // Draw
            RenderCommand::DrawIndexed(s_Data.QuadIndexBuffer, 6);
        }
    }

    // =========================================================================
    // Draw Primitives - Color
    // =========================================================================

    void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const vec4& color)
    {
        DrawQuad(vec3(position, 0.0f), size, color);
    }

    void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color)
    {
        DrawQuadInternal(position, size, nullptr, color, 1.0f);
    }

    // =========================================================================
    // Draw Primitives - Texture
    // =========================================================================

    void Renderer2D::DrawQuad(const vec2& position, const vec2& size, Texture2D* texture,
                              float32 tilingFactor)
    {
        DrawQuad(vec3(position, 0.0f), size, texture, tilingFactor);
    }

    void Renderer2D::DrawQuad(const vec3& position, const vec2& size, Texture2D* texture,
                              float32 tilingFactor)
    {
        constexpr vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
        DrawQuadInternal(position, size, texture, white, tilingFactor);
    }

    // =========================================================================
    // Draw Primitives - Texture + Tint
    // =========================================================================

    void Renderer2D::DrawQuad(const vec2& position, const vec2& size, Texture2D* texture,
                              const vec4& tintColor, float32 tilingFactor)
    {
        DrawQuad(vec3(position, 0.0f), size, texture, tintColor, tilingFactor);
    }

    void Renderer2D::DrawQuad(const vec3& position, const vec2& size, Texture2D* texture,
                              const vec4& tintColor, float32 tilingFactor)
    {
        DrawQuadInternal(position, size, texture, tintColor, tilingFactor);
    }

    // =========================================================================
    // Draw Primitives - Rotated + Color
    // =========================================================================

    void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size,
                                     float32 rotation, const vec4& color)
    {
        DrawRotatedQuad(vec3(position, 0.0f), size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size,
                                     float32 rotation, const vec4& color)
    {
        DrawRotatedQuadInternal(position, size, rotation, nullptr, color, 1.0f);
    }

    // =========================================================================
    // Draw Primitives - Rotated + Texture
    // =========================================================================

    void Renderer2D::DrawRotatedQuad(const vec2& position, const vec2& size,
                                     float32 rotation, Texture2D* texture,
                                     float32 tilingFactor, const vec4& tintColor)
    {
        DrawRotatedQuad(vec3(position, 0.0f), size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const vec3& position, const vec2& size,
                                     float32 rotation, Texture2D* texture,
                                     float32 tilingFactor, const vec4& tintColor)
    {
        DrawRotatedQuadInternal(position, size, rotation, texture, tintColor, tilingFactor);
    }

} // namespace NanSu

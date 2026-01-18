#include "EnginePCH.h"
#include "Platform/Windows/DX11RendererAPI.h"

#ifdef NS_PLATFORM_WINDOWS

#include "Core/Application.h"
#include "Renderer/Buffer.h"

#include <d3d11.h>

namespace NanSu
{
    // =========================================================================
    // Factory Method Implementation
    // =========================================================================

    RendererAPI* RendererAPI::Create()
    {
        return new DX11RendererAPI();
    }

    // =========================================================================
    // Helper Functions
    // =========================================================================

    namespace
    {
        D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopologyToDX11(PrimitiveTopology topology)
        {
            switch (topology)
            {
                case PrimitiveTopology::PointList:      return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
                case PrimitiveTopology::LineList:       return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
                case PrimitiveTopology::LineStrip:      return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
                case PrimitiveTopology::TriangleList:   return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case PrimitiveTopology::TriangleStrip:  return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                case PrimitiveTopology::None:
                default:
                    NS_ENGINE_ASSERT(false, "Invalid PrimitiveTopology");
                    return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
            }
        }
    }

    // =========================================================================
    // DX11RendererAPI Implementation
    // =========================================================================

    void DX11RendererAPI::Init()
    {
        NS_ENGINE_INFO("Initializing DirectX 11 Renderer API");

        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Create blend state for alpha blending
        {
            D3D11_BLEND_DESC blendDesc = {};
            blendDesc.AlphaToCoverageEnable = FALSE;
            blendDesc.IndependentBlendEnable = FALSE;
            blendDesc.RenderTarget[0].BlendEnable = TRUE;
            blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

            HRESULT hr = device->CreateBlendState(&blendDesc, &m_BlendState);
            NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create blend state");

            // Bind the blend state
            float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            deviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xFFFFFFFF);
        }

        NS_ENGINE_INFO("DirectX 11 Renderer API initialized");
    }

    void DX11RendererAPI::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down DirectX 11 Renderer API");

        if (m_BlendState)
        {
            m_BlendState->Release();
            m_BlendState = nullptr;
        }
    }

    void DX11RendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = static_cast<float32>(x);
        viewport.TopLeftY = static_cast<float32>(y);
        viewport.Width = static_cast<float32>(width);
        viewport.Height = static_cast<float32>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        deviceContext->RSSetViewports(1, &viewport);
    }

    void DX11RendererAPI::SetClearColor(float32 r, float32 g, float32 b, float32 a)
    {
        m_ClearColor[0] = r;
        m_ClearColor[1] = g;
        m_ClearColor[2] = b;
        m_ClearColor[3] = a;
    }

    void DX11RendererAPI::Clear()
    {
        Application::Get().GetGraphicsContext().Clear(
            m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    }

    void DX11RendererAPI::SetPrimitiveTopology(PrimitiveTopology topology)
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->IASetPrimitiveTopology(PrimitiveTopologyToDX11(topology));
    }

    void DX11RendererAPI::BindRenderTarget()
    {
        Application::Get().GetGraphicsContext().BindRenderTarget();

        // Re-bind blend state (ImGui may have changed it)
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        deviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xFFFFFFFF);
    }

    void DX11RendererAPI::DrawIndexed(const IndexBuffer* indexBuffer, uint32 indexCount)
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        uint32 count = indexCount ? indexCount : indexBuffer->GetCount();
        deviceContext->DrawIndexed(count, 0, 0);
    }

}

#endif

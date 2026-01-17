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

        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        NS_ENGINE_INFO("DirectX 11 Renderer API initialized");
    }

    void DX11RendererAPI::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down DirectX 11 Renderer API");
    }

    void DX11RendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = static_cast<f32>(x);
        viewport.TopLeftY = static_cast<f32>(y);
        viewport.Width = static_cast<f32>(width);
        viewport.Height = static_cast<f32>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        deviceContext->RSSetViewports(1, &viewport);
    }

    void DX11RendererAPI::SetClearColor(f32 r, f32 g, f32 b, f32 a)
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

#include "EnginePCH.h"
#include "Platform/Windows/DX11Buffer.h"

#ifdef NS_PLATFORM_WINDOWS

#include "Core/Application.h"

#include <d3d11.h>

namespace NanSu
{
    // =========================================================================
    // Factory Methods
    // =========================================================================

    VertexBuffer* VertexBuffer::Create(const void* vertices, uint32 size)
    {
        return new DX11VertexBuffer(vertices, size);
    }

    IndexBuffer* IndexBuffer::Create(const uint32* indices, uint32 count)
    {
        return new DX11IndexBuffer(indices, count);
    }

    // =========================================================================
    // DX11VertexBuffer
    // =========================================================================

    DX11VertexBuffer::DX11VertexBuffer(const void* vertices, uint32 size)
    {
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices;

        HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_Buffer);
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");

        NS_ENGINE_INFO("Vertex buffer created (size: {} bytes)", size);
    }

    DX11VertexBuffer::~DX11VertexBuffer()
    {
        if (m_Buffer)
        {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
    }

    void DX11VertexBuffer::Bind() const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        UINT stride = m_Layout.GetStride();
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);
    }

    void DX11VertexBuffer::Unbind() const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        ID3D11Buffer* nullBuffer = nullptr;
        UINT stride = 0;
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
    }

    // =========================================================================
    // DX11IndexBuffer
    // =========================================================================

    DX11IndexBuffer::DX11IndexBuffer(const uint32* indices, uint32 count)
        : m_Count(count)
    {
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = count * sizeof(uint32);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices;

        HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_Buffer);
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create index buffer");

        NS_ENGINE_INFO("Index buffer created (count: {})", count);
    }

    DX11IndexBuffer::~DX11IndexBuffer()
    {
        if (m_Buffer)
        {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
    }

    void DX11IndexBuffer::Bind() const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
    }

    void DX11IndexBuffer::Unbind() const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
    }

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

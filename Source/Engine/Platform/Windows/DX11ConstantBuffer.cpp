#include "EnginePCH.h"
#include "Platform/Windows/DX11ConstantBuffer.h"

#ifdef NS_PLATFORM_WINDOWS

#include "Core/Application.h"

#include <d3d11.h>

namespace NanSu
{
    // =========================================================================
    // Factory Method
    // =========================================================================

    ConstantBuffer* ConstantBuffer::Create(uint32 size)
    {
        return new DX11ConstantBuffer(size);
    }

    // =========================================================================
    // DX11ConstantBuffer
    // =========================================================================

    DX11ConstantBuffer::DX11ConstantBuffer(uint32 size)
    {
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        // DX11 requires constant buffer size to be multiple of 16 bytes
        uint32 alignedSize = (size + 15) & ~15;
        m_Size = alignedSize;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = alignedSize;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &m_Buffer);
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create constant buffer");

        NS_ENGINE_INFO("Constant buffer created (size: {} bytes, aligned: {} bytes)", size, alignedSize);
    }

    DX11ConstantBuffer::~DX11ConstantBuffer()
    {
        if (m_Buffer)
        {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
    }

    void DX11ConstantBuffer::SetData(const void* data, uint32 size)
    {
        NS_ENGINE_ASSERT(size <= m_Size, "Data size ({}) exceeds buffer size ({})", size, m_Size);
        NS_ENGINE_ASSERT(data != nullptr, "Data pointer is null");

        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = deviceContext->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to map constant buffer");

        memcpy(mappedResource.pData, data, size);

        deviceContext->Unmap(m_Buffer, 0);
    }

    void DX11ConstantBuffer::Bind(uint32 slot) const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        // Bind to both vertex shader and pixel shader
        deviceContext->VSSetConstantBuffers(slot, 1, &m_Buffer);
        deviceContext->PSSetConstantBuffers(slot, 1, &m_Buffer);
    }

    void DX11ConstantBuffer::Unbind(uint32 slot) const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        ID3D11Buffer* nullBuffer = nullptr;
        deviceContext->VSSetConstantBuffers(slot, 1, &nullBuffer);
        deviceContext->PSSetConstantBuffers(slot, 1, &nullBuffer);
    }

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

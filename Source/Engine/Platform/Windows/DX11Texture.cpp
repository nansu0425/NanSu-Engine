#include "EnginePCH.h"
#include "Platform/Windows/DX11Texture.h"

#ifdef NS_PLATFORM_WINDOWS

#include "Core/Application.h"

#include <d3d11.h>
#include <stb_image.h>

namespace NanSu
{
    // =========================================================================
    // Factory Methods
    // =========================================================================

    Texture2D* Texture2D::Create(const std::string& filePath)
    {
        return new DX11Texture2D(filePath);
    }

    Texture2D* Texture2D::Create(uint32 width, uint32 height)
    {
        return new DX11Texture2D(width, height);
    }

    // =========================================================================
    // DX11Texture2D
    // =========================================================================

    DX11Texture2D::DX11Texture2D(const std::string& filePath)
        : m_FilePath(filePath)
    {
        // Flip vertically for DirectX coordinate system (top-left origin)
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        stbi_uc* data = stbi_load(
            filePath.c_str(),
            &width,
            &height,
            &channels,
            STBI_rgb_alpha  // Force 4 channels (RGBA)
        );

        if (!data)
        {
            NS_ENGINE_ERROR("Failed to load texture: {}", filePath);
            NS_ENGINE_ERROR("stbi_failure_reason: {}", stbi_failure_reason());
            return;
        }

        m_Width = static_cast<uint32>(width);
        m_Height = static_cast<uint32>(height);

        CreateTexture(data);
        CreateSampler();

        stbi_image_free(data);

        NS_ENGINE_INFO("Texture loaded: {} ({}x{}, {} channels)",
                       filePath, m_Width, m_Height, channels);
    }

    DX11Texture2D::DX11Texture2D(uint32 width, uint32 height)
        : m_Width(width)
        , m_Height(height)
    {
        CreateTexture(nullptr);
        CreateSampler();

        NS_ENGINE_INFO("Empty texture created ({}x{})", width, height);
    }

    DX11Texture2D::~DX11Texture2D()
    {
        if (m_SamplerState)
        {
            m_SamplerState->Release();
            m_SamplerState = nullptr;
        }

        if (m_ShaderResourceView)
        {
            m_ShaderResourceView->Release();
            m_ShaderResourceView = nullptr;
        }

        if (m_Texture)
        {
            m_Texture->Release();
            m_Texture = nullptr;
        }
    }

    void DX11Texture2D::Bind(uint32 slot) const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
        deviceContext->PSSetSamplers(slot, 1, &m_SamplerState);
    }

    void DX11Texture2D::Unbind(uint32 slot) const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        ID3D11ShaderResourceView* nullSRV = nullptr;
        ID3D11SamplerState* nullSampler = nullptr;
        deviceContext->PSSetShaderResources(slot, 1, &nullSRV);
        deviceContext->PSSetSamplers(slot, 1, &nullSampler);
    }

    void DX11Texture2D::SetData(const void* data, uint32 size)
    {
        uint32 expectedSize = m_Width * m_Height * 4;  // RGBA = 4 bytes per pixel
        NS_ENGINE_ASSERT(size == expectedSize,
            "Data size ({}) does not match texture size ({})", size, expectedSize);

        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        // Update texture data using UpdateSubresource
        D3D11_BOX destBox = {};
        destBox.left = 0;
        destBox.right = m_Width;
        destBox.top = 0;
        destBox.bottom = m_Height;
        destBox.front = 0;
        destBox.back = 1;

        uint32 rowPitch = m_Width * 4;
        deviceContext->UpdateSubresource(m_Texture, 0, &destBox, data, rowPitch, 0);
    }

    void DX11Texture2D::CreateTexture(const void* data)
    {
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        // Texture description
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        HRESULT hr;

        if (data)
        {
            // Initialize with data
            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = data;
            initData.SysMemPitch = m_Width * 4;  // 4 bytes per pixel (RGBA)

            hr = device->CreateTexture2D(&textureDesc, &initData, &m_Texture);
        }
        else
        {
            // Create empty texture
            hr = device->CreateTexture2D(&textureDesc, nullptr, &m_Texture);
        }

        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create texture");

        // Create Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        hr = device->CreateShaderResourceView(m_Texture, &srvDesc, &m_ShaderResourceView);
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create shader resource view");
    }

    void DX11Texture2D::CreateSampler()
    {
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  // Trilinear filtering
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.BorderColor[0] = 0.0f;
        samplerDesc.BorderColor[1] = 0.0f;
        samplerDesc.BorderColor[2] = 0.0f;
        samplerDesc.BorderColor[3] = 0.0f;
        samplerDesc.MinLOD = 0.0f;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        HRESULT hr = device->CreateSamplerState(&samplerDesc, &m_SamplerState);
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create sampler state");
    }

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

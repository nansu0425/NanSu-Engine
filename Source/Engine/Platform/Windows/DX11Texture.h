#pragma once

#include "Renderer/Texture.h"

#ifdef NS_PLATFORM_WINDOWS

// Forward declarations to avoid including DX11 headers
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace NanSu
{
    /**
     * @brief DirectX 11 implementation of Texture2D
     *
     * Manages ID3D11Texture2D, ID3D11ShaderResourceView, and ID3D11SamplerState.
     */
    class DX11Texture2D : public Texture2D
    {
    public:
        /**
         * @brief Create a texture from a file path
         * @param filePath Path to the image file
         */
        DX11Texture2D(const std::string& filePath);

        /**
         * @brief Create an empty texture with specified dimensions
         * @param width Texture width in pixels
         * @param height Texture height in pixels
         */
        DX11Texture2D(uint32 width, uint32 height);

        ~DX11Texture2D();

        // Texture interface
        uint32 GetWidth() const override { return m_Width; }
        uint32 GetHeight() const override { return m_Height; }
        void Bind(uint32 slot = 0) const override;
        void Unbind(uint32 slot = 0) const override;

        // Texture2D interface
        void SetData(const void* data, uint32 size) override;

    private:
        /**
         * @brief Create the texture and shader resource view from raw data
         * @param data RGBA pixel data (can be nullptr for empty texture)
         */
        void CreateTexture(const void* data);

        /**
         * @brief Create a sampler state with default settings
         */
        void CreateSampler();

    private:
        std::string m_FilePath;
        uint32 m_Width = 0;
        uint32 m_Height = 0;

        // DX11 resources
        ID3D11Texture2D* m_Texture = nullptr;
        ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
        ID3D11SamplerState* m_SamplerState = nullptr;
    };

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

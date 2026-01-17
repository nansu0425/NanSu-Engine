#pragma once

#include "Renderer/RendererAPI.h"

#ifdef NS_PLATFORM_WINDOWS

namespace NanSu
{
    /**
     * @brief DirectX 11 implementation of RendererAPI
     *
     * Provides low-level rendering commands using the DX11 graphics API.
     * Accesses the device context through Application::GetGraphicsContext().
     */
    class DX11RendererAPI : public RendererAPI
    {
    public:
        DX11RendererAPI() = default;
        ~DX11RendererAPI() override = default;

        void Init() override;
        void Shutdown() override;

        void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f) override;
        void Clear() override;
        void SetPrimitiveTopology(PrimitiveTopology topology) override;
        void BindRenderTarget() override;
        void DrawIndexed(const IndexBuffer* indexBuffer, uint32 indexCount = 0) override;

    private:
        f32 m_ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    };

}

#endif

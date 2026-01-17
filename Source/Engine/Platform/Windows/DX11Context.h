#pragma once

#include "Renderer/GraphicsContext.h"

#ifdef NS_PLATFORM_WINDOWS

// Forward declarations to avoid including DX11 headers in the header
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace NanSu
{
    /**
     * @brief DirectX 11 implementation of GraphicsContext
     *
     * Creates and manages DX11 device, swap chain, and render targets.
     * Handles frame presentation and window resize.
     */
    class DX11Context : public GraphicsContext
    {
    public:
        /**
         * @brief Construct a DX11 context for the given window
         * @param hwnd Windows window handle
         * @param width Window client width
         * @param height Window client height
         */
        DX11Context(void* hwnd, uint32 width, uint32 height);
        ~DX11Context();

        bool Init() override;
        void Shutdown() override;
        void Clear(f32 r, f32 g, f32 b, f32 a = 1.0f) override;
        void SwapBuffers() override;
        void OnResize(uint32 width, uint32 height) override;

        void* GetNativeDevice() const override { return m_Device; }
        void* GetNativeDeviceContext() const override { return m_DeviceContext; }
        void BindRenderTarget() override;

        // DX11-specific getters (for engine internals that need typed access)
        ID3D11Device* GetDevice() const { return m_Device; }
        ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }
        IDXGISwapChain* GetSwapChain() const { return m_SwapChain; }

    private:
        /**
         * @brief Create render target view from swap chain back buffer
         */
        bool CreateRenderTargetView();

        /**
         * @brief Release render target view (called before resize)
         */
        void ReleaseRenderTargetView();

    private:
        HWND m_Hwnd = nullptr;
        uint32 m_Width = 0;
        uint32 m_Height = 0;

        // Core DX11 objects
        ID3D11Device* m_Device = nullptr;
        ID3D11DeviceContext* m_DeviceContext = nullptr;
        IDXGISwapChain* m_SwapChain = nullptr;

        // Render targets
        ID3D11RenderTargetView* m_RenderTargetView = nullptr;
    };
}

#endif // NS_PLATFORM_WINDOWS

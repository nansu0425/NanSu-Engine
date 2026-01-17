#include "EnginePCH.h"
#include "Platform/Windows/DX11Context.h"

#ifdef NS_PLATFORM_WINDOWS

#include <d3d11.h>
#include <dxgi.h>

// Link required libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace NanSu
{
    // Factory method implementation
    GraphicsContext* GraphicsContext::Create(void* windowHandle, uint32 width, uint32 height)
    {
        return new DX11Context(windowHandle, width, height);
    }

    DX11Context::DX11Context(void* hwnd, uint32 width, uint32 height)
        : m_Hwnd(static_cast<HWND>(hwnd))
        , m_Width(width)
        , m_Height(height)
    {
    }

    DX11Context::~DX11Context()
    {
        Shutdown();
    }

    bool DX11Context::Init()
    {
        NS_ENGINE_INFO("Initializing DirectX 11 context ({}x{})", m_Width, m_Height);

        // Describe the swap chain
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = 2;  // Double buffering
        swapChainDesc.BufferDesc.Width = m_Width;
        swapChainDesc.BufferDesc.Height = m_Height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = m_Hwnd;
        swapChainDesc.SampleDesc.Count = 1;      // No MSAA
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;  // Modern swap effect
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // Feature levels to try (ordered by preference)
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        // Device creation flags
        UINT createDeviceFlags = 0;
#ifdef NS_DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;  // Enable debug layer
#endif

        D3D_FEATURE_LEVEL featureLevelObtained;

        // Create device and swap chain
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,                    // Use default adapter
            D3D_DRIVER_TYPE_HARDWARE,   // Hardware acceleration
            nullptr,                    // No software rasterizer
            createDeviceFlags,          // Creation flags
            featureLevels,              // Feature levels to try
            numFeatureLevels,           // Number of feature levels
            D3D11_SDK_VERSION,          // SDK version
            &swapChainDesc,             // Swap chain description
            &m_SwapChain,               // [out] Swap chain
            &m_Device,                  // [out] Device
            &featureLevelObtained,      // [out] Feature level obtained
            &m_DeviceContext            // [out] Device context
        );

        if (FAILED(hr))
        {
            NS_ENGINE_CRITICAL("Failed to create D3D11 device and swap chain. HRESULT: {:#x}",
                              static_cast<uint32>(hr));
            return false;
        }

        NS_ENGINE_INFO("DirectX 11 device created successfully");
        NS_ENGINE_INFO("  Feature Level: {}.{}",
                      (featureLevelObtained >> 12) & 0xF,
                      (featureLevelObtained >> 8) & 0xF);

        // Create render target view
        if (!CreateRenderTargetView())
        {
            NS_ENGINE_CRITICAL("Failed to create render target view");
            Shutdown();
            return false;
        }

        // Set the viewport
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<f32>(m_Width);
        viewport.Height = static_cast<f32>(m_Height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        m_DeviceContext->RSSetViewports(1, &viewport);

        // Bind render target
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);

        NS_ENGINE_INFO("DirectX 11 context initialized successfully");
        return true;
    }

    void DX11Context::Shutdown()
    {
        NS_ENGINE_INFO("Shutting down DirectX 11 context");

        ReleaseRenderTargetView();

        if (m_SwapChain)
        {
            m_SwapChain->Release();
            m_SwapChain = nullptr;
        }

        if (m_DeviceContext)
        {
            m_DeviceContext->Release();
            m_DeviceContext = nullptr;
        }

#ifdef NS_DEBUG
        // Report live objects in debug mode
        if (m_Device)
        {
            ID3D11Debug* debugDevice = nullptr;
            if (SUCCEEDED(m_Device->QueryInterface(__uuidof(ID3D11Debug),
                         reinterpret_cast<void**>(&debugDevice))))
            {
                debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
                debugDevice->Release();
            }
        }
#endif

        if (m_Device)
        {
            m_Device->Release();
            m_Device = nullptr;
        }

        NS_ENGINE_INFO("DirectX 11 context shut down");
    }

    void DX11Context::Clear(f32 r, f32 g, f32 b, f32 a)
    {
        f32 clearColor[4] = { r, g, b, a };
        m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
    }

    void DX11Context::SwapBuffers()
    {
        // Present with vsync (1) or without (0)
        HRESULT hr = m_SwapChain->Present(1, 0);  // VSync enabled

        if (FAILED(hr))
        {
            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
            {
                NS_ENGINE_ERROR("Device lost! HRESULT: {:#x}", static_cast<uint32>(hr));
                // TODO: Handle device lost - recreate device
            }
        }
    }

    void DX11Context::OnResize(uint32 width, uint32 height)
    {
        if (width == 0 || height == 0)
        {
            return;  // Window is minimized
        }

        if (width == m_Width && height == m_Height)
        {
            return;  // No change
        }

        NS_ENGINE_INFO("Resizing DirectX 11 swap chain to {}x{}", width, height);

        m_Width = width;
        m_Height = height;

        // Release render target before resizing
        ReleaseRenderTargetView();

        // Resize swap chain buffers
        HRESULT hr = m_SwapChain->ResizeBuffers(
            0,                              // Keep buffer count
            width, height,                  // New dimensions
            DXGI_FORMAT_UNKNOWN,            // Keep format
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        );

        if (FAILED(hr))
        {
            NS_ENGINE_ERROR("Failed to resize swap chain buffers. HRESULT: {:#x}",
                           static_cast<uint32>(hr));
            return;
        }

        // Recreate render target view
        CreateRenderTargetView();

        // Update viewport
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<f32>(width);
        viewport.Height = static_cast<f32>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        m_DeviceContext->RSSetViewports(1, &viewport);

        // Rebind render target
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);

        NS_ENGINE_INFO("DirectX 11 swap chain resized successfully");
    }

    bool DX11Context::CreateRenderTargetView()
    {
        // Get back buffer from swap chain
        ID3D11Texture2D* backBuffer = nullptr;
        HRESULT hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                            reinterpret_cast<void**>(&backBuffer));

        if (FAILED(hr))
        {
            NS_ENGINE_ERROR("Failed to get back buffer. HRESULT: {:#x}",
                           static_cast<uint32>(hr));
            return false;
        }

        // Create render target view
        hr = m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
        backBuffer->Release();

        if (FAILED(hr))
        {
            NS_ENGINE_ERROR("Failed to create render target view. HRESULT: {:#x}",
                           static_cast<uint32>(hr));
            return false;
        }

        return true;
    }

    void DX11Context::ReleaseRenderTargetView()
    {
        if (m_DeviceContext)
        {
            // Unbind render target before releasing
            m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
        }

        if (m_RenderTargetView)
        {
            m_RenderTargetView->Release();
            m_RenderTargetView = nullptr;
        }
    }
}

#endif // NS_PLATFORM_WINDOWS

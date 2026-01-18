#include "EnginePCH.h"
#include "UI/ImGuiLayer.h"
#include "Core/Application.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <d3d11.h>

namespace NanSu
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    void ImGuiLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled, tweak WindowRounding/WindowBg
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Get window and graphics context
        Application& app = Application::Get();
        HWND hwnd = static_cast<HWND>(app.GetWindow().GetNativeWindow());

        // Get DX11 device and context
        auto* device = static_cast<ID3D11Device*>(
            app.GetGraphicsContext().GetNativeDevice());
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            app.GetGraphicsContext().GetNativeDeviceContext());

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(device, deviceContext);

        NS_ENGINE_INFO("ImGuiLayer attached");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        NS_ENGINE_INFO("ImGuiLayer detached");
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(
            static_cast<float>(app.GetWindow().GetWidth()),
            static_cast<float>(app.GetWindow().GetHeight())
        );

        // Rendering
        ImGui::Render();

        // Bind the main render target before rendering ImGui
        app.GetGraphicsContext().BindRenderTarget();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            bool handled = event.IsHandled();
            handled |= (event.IsInCategory(EventCategoryMouse) && io.WantCaptureMouse);
            handled |= (event.IsInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard);
            event.SetHandled(handled);
        }
    }
}

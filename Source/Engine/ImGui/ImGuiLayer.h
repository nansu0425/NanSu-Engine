#pragma once

#include "Core/Layer.h"

namespace NanSu
{
    /**
     * @brief ImGui overlay layer for debug UI rendering
     *
     * Manages ImGui initialization, frame lifecycle, and input handling.
     * Should be pushed as an overlay to ensure it renders on top and
     * receives events first.
     */
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;

        /**
         * @brief Begin a new ImGui frame
         * Must be called once at the start of the frame before any ImGui calls
         */
        void Begin();

        /**
         * @brief End the ImGui frame and render
         * Must be called once at the end of the frame after all ImGui calls
         */
        void End();

        /**
         * @brief Block events from propagating when ImGui wants capture
         */
        void SetBlockEvents(bool block) { m_BlockEvents = block; }

    private:
        bool m_BlockEvents = true;
    };
}

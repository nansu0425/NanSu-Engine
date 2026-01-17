#pragma once

#include "Core/Types.h"
#include "Core/Layer.h"
#include <vector>

namespace NanSu
{
    /**
     * @brief Container managing application layers with proper ordering
     *
     * The LayerStack maintains two logical sections:
     * - Regular layers (bottom): Pushed at the insert point, before overlays
     * - Overlays (top): Always on top, pushed at the end
     *
     * Stack layout:
     *   [Layer0, Layer1, ..., LayerN | Overlay0, Overlay1, ..., OverlayN]
     *                                ^
     *                       m_LayerInsertIndex
     *
     * Update order: bottom to top (Layer0 first, OverlayN last)
     * Event order: top to bottom (OverlayN first, Layer0 last)
     */
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        /**
         * @brief Push a regular layer onto the stack
         * @param layer The layer to push (LayerStack takes ownership)
         *
         * Layer is inserted before all overlays. OnAttach() is called.
         */
        void PushLayer(Layer* layer);

        /**
         * @brief Pop a regular layer from the stack
         * @param layer The layer to pop
         *
         * OnDetach() is called. Caller regains ownership of the layer.
         */
        void PopLayer(Layer* layer);

        /**
         * @brief Push an overlay onto the stack
         * @param overlay The overlay to push (LayerStack takes ownership)
         *
         * Overlay is inserted at the end (on top). OnAttach() is called.
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief Pop an overlay from the stack
         * @param overlay The overlay to pop
         *
         * OnDetach() is called. Caller regains ownership of the overlay.
         */
        void PopOverlay(Layer* overlay);

        // Iterator support for forward traversal (update order)
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
        std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
        std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }

        // Iterator support for reverse traversal (event order)
        std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    private:
        std::vector<Layer*> m_Layers;
        usize m_LayerInsertIndex = 0;
    };
}

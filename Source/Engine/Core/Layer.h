#pragma once

#include "Core/Types.h"
#include "Events/Event.h"
#include <string>

namespace NanSu
{
    /**
     * @brief Base class for application layers
     *
     * Layers represent distinct logical sections of the application such as
     * game logic, UI systems, or debug overlays. Each layer receives updates
     * and events in a defined order.
     *
     * Update order: bottom to top (game world -> UI)
     * Event order: top to bottom (UI -> game world, UI can consume events)
     */
    class Layer
    {
    public:
        /**
         * @brief Construct a layer with a debug name
         * @param name Debug name for identification (default: "Layer")
         */
        explicit Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        /**
         * @brief Called when the layer is pushed onto the stack
         * Use for initialization, resource loading, etc.
         */
        virtual void OnAttach() {}

        /**
         * @brief Called when the layer is popped from the stack
         * Use for cleanup, resource release, etc.
         */
        virtual void OnDetach() {}

        /**
         * @brief Called every frame
         * Update logic should go here. Layers are updated bottom to top.
         */
        virtual void OnUpdate() {}

        /**
         * @brief Called when an event is propagated to this layer
         * @param event The event to handle
         *
         * Events are propagated top to bottom (overlays first).
         * Set event.SetHandled(true) to stop propagation.
         */
        virtual void OnEvent(Event& event) {}

        /**
         * @brief Get the debug name of this layer
         * @return The layer's debug name
         */
        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}

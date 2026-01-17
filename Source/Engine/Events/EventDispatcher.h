#pragma once

#include "Events/Event.h"

namespace NanSu
{
    /**
     * @brief Type-safe event dispatcher for local event handling
     *
     * Usage:
     *   void OnWindowResize(WindowResizeEvent& event) { ... return true; }
     *
     *   void OnEvent(Event& event)
     *   {
     *       EventDispatcher dispatcher(event);
     *       dispatcher.Dispatch<WindowResizeEvent>(NS_BIND_EVENT_FN(OnWindowResize));
     *   }
     */
    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event)
            : m_Event(event)
        {
        }

        /**
         * @brief Dispatch event to handler if types match
         * @tparam T The event type to dispatch
         * @tparam F The handler function type
         * @param handler Function that takes T& and returns bool
         * @return true if the event was dispatched (type matched)
         */
        template<typename T, typename F>
        bool Dispatch(const F& handler)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.SetHandled(handler(static_cast<T&>(m_Event)));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    // Helper macro for binding member function event handlers
    #define NS_BIND_EVENT_FN(fn) [this](auto&& event) { return this->fn(std::forward<decltype(event)>(event)); }
}

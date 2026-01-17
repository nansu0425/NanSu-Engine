#pragma once

#include <string>
#include <cstdint>
#include <functional>

namespace NanSu
{
    /**
     * @brief Event type enum for runtime type identification
     */
    enum class EventType : uint32_t
    {
        None = 0,

        // Application Events
        AppInit,
        AppShutdown,
        AppUpdate,

        // Window Events
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,

        // Key Events
        KeyPressed,
        KeyReleased,
        KeyTyped,

        // Mouse Events
        MouseMoved,
        MouseScrolled,
        MouseButtonPressed,
        MouseButtonReleased
    };

    /**
     * @brief Event category flags for filtering (can be combined with bitwise OR)
     */
    enum EventCategory : uint32_t
    {
        EventCategoryNone        = 0,
        EventCategoryApplication = 1 << 0,
        EventCategoryWindow      = 1 << 1,
        EventCategoryInput       = 1 << 2,
        EventCategoryKeyboard    = 1 << 3,
        EventCategoryMouse       = 1 << 4,
        EventCategoryMouseButton = 1 << 5
    };

    // Macro to reduce boilerplate in event classes
    #define NS_EVENT_CLASS_TYPE(type) \
        static EventType GetStaticType() { return EventType::type; } \
        virtual EventType GetEventType() const override { return GetStaticType(); } \
        virtual const char* GetName() const override { return #type; }

    #define NS_EVENT_CLASS_CATEGORY(category) \
        virtual uint32_t GetCategoryFlags() const override { return category; }

    /**
     * @brief Base class for all events in the engine
     */
    class Event
    {
    public:
        virtual ~Event() = default;

        // Pure virtual methods for type identification
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual uint32_t GetCategoryFlags() const = 0;

        // Optional: String representation for debugging
        virtual std::string ToString() const { return GetName(); }

        // Check if event belongs to a category
        bool IsInCategory(EventCategory category) const
        {
            return GetCategoryFlags() & category;
        }

        // Handled flag for propagation control
        bool IsHandled() const { return m_Handled; }
        void SetHandled(bool handled) { m_Handled = handled; }

    protected:
        bool m_Handled = false;
    };

    // Type alias for event callbacks
    using EventCallbackFn = std::function<void(Event&)>;
}

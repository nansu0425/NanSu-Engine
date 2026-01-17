#pragma once

#include "Events/Event.h"
#include <vector>
#include <unordered_map>
#include <functional>

namespace NanSu
{
    /**
     * @brief Static event bus for global event subscription and publishing
     *
     * Follows the same pattern as Logger - static subsystem with Initialize/Shutdown
     */
    class EventBus
    {
    public:
        using EventHandler = std::function<void(Event&)>;
        using HandlerId = uint64_t;

        // Lifecycle (matches Logger pattern)
        static void Initialize();
        static void Shutdown();

        /**
         * @brief Subscribe to all events of a specific type
         * @tparam T The event type to subscribe to
         * @param handler The callback function
         * @return Handler ID for later unsubscription
         */
        template<typename T>
        static HandlerId Subscribe(std::function<void(T&)> handler)
        {
            HandlerId id = s_NextHandlerId++;
            EventType type = T::GetStaticType();

            s_Handlers[type].push_back({
                id,
                [handler](Event& event)
                {
                    handler(static_cast<T&>(event));
                }
            });

            return id;
        }

        /**
         * @brief Subscribe to events by category
         * @param category The event category to subscribe to
         * @param handler The callback function
         * @return Handler ID for later unsubscription
         */
        static HandlerId SubscribeToCategory(EventCategory category, EventHandler handler);

        /**
         * @brief Unsubscribe a handler by ID
         * @param handlerId The handler ID returned from Subscribe
         */
        static void Unsubscribe(HandlerId handlerId);

        /**
         * @brief Publish an event to all subscribers
         * @param event The event to publish
         */
        static void Publish(Event& event);

        /**
         * @brief Publish an event by value (convenience method)
         * @tparam T The event type
         * @param event The event to publish
         */
        template<typename T>
        static void Publish(T event)
        {
            Publish(static_cast<Event&>(event));
        }

    private:
        struct HandlerEntry
        {
            HandlerId id;
            EventHandler handler;
        };

        struct CategoryHandlerEntry
        {
            HandlerId id;
            EventCategory category;
            EventHandler handler;
        };

        static std::unordered_map<EventType, std::vector<HandlerEntry>> s_Handlers;
        static std::vector<CategoryHandlerEntry> s_CategoryHandlers;
        static HandlerId s_NextHandlerId;
        static bool s_Initialized;
    };
}

// Logging macros for event debugging (optional, only in debug builds)
#ifdef NS_DEBUG
    #define NS_EVENT_TRACE(event) NS_ENGINE_TRACE("[Event] {}", event.ToString())
#else
    #define NS_EVENT_TRACE(event)
#endif

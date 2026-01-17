#include "EnginePCH.h"
#include "Events/EventBus.h"

namespace NanSu
{
    std::unordered_map<EventType, std::vector<EventBus::HandlerEntry>> EventBus::s_Handlers;
    std::vector<EventBus::CategoryHandlerEntry> EventBus::s_CategoryHandlers;
    EventBus::HandlerId EventBus::s_NextHandlerId = 1;
    bool EventBus::s_Initialized = false;

    void EventBus::Initialize()
    {
        if (!s_Initialized)
        {
            s_Handlers.clear();
            s_CategoryHandlers.clear();
            s_NextHandlerId = 1;
            s_Initialized = true;
            NS_ENGINE_INFO("EventBus initialized");
        }
    }

    void EventBus::Shutdown()
    {
        if (s_Initialized)
        {
            NS_ENGINE_INFO("EventBus shutting down");
            s_Handlers.clear();
            s_CategoryHandlers.clear();
            s_Initialized = false;
        }
    }

    EventBus::HandlerId EventBus::SubscribeToCategory(EventCategory category, EventHandler handler)
    {
        NS_ENGINE_ASSERT(s_Initialized, "EventBus must be initialized before subscribing");
        NS_ENGINE_ASSERT(handler != nullptr, "Event handler cannot be null");

        HandlerId id = s_NextHandlerId++;
        s_CategoryHandlers.push_back({ id, category, std::move(handler) });
        return id;
    }

    void EventBus::Unsubscribe(HandlerId handlerId)
    {
        // Remove from type-specific handlers
        for (auto& [type, handlers] : s_Handlers)
        {
            handlers.erase(
                std::remove_if(handlers.begin(), handlers.end(),
                    [handlerId](const HandlerEntry& entry)
                    {
                        return entry.id == handlerId;
                    }),
                handlers.end()
            );
        }

        // Remove from category handlers
        s_CategoryHandlers.erase(
            std::remove_if(s_CategoryHandlers.begin(), s_CategoryHandlers.end(),
                [handlerId](const CategoryHandlerEntry& entry)
                {
                    return entry.id == handlerId;
                }),
            s_CategoryHandlers.end()
        );
    }

    void EventBus::Publish(Event& event)
    {
        NS_ENGINE_ASSERT(s_Initialized, "EventBus must be initialized before publishing events");
        NS_EVENT_TRACE(event);

        // Dispatch to type-specific handlers
        EventType type = event.GetEventType();
        auto it = s_Handlers.find(type);
        if (it != s_Handlers.end())
        {
            for (auto& entry : it->second)
            {
                if (event.IsHandled())
                {
                    break;
                }
                entry.handler(event);
            }
        }

        // Dispatch to category handlers
        for (auto& entry : s_CategoryHandlers)
        {
            if (event.IsHandled())
            {
                break;
            }
            if (event.IsInCategory(entry.category))
            {
                entry.handler(event);
            }
        }
    }
}

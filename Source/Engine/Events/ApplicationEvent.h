#pragma once

#include "Events/Event.h"
#include <sstream>

namespace NanSu
{
    /**
     * @brief Event fired when the application initializes
     */
    class AppInitEvent : public Event
    {
    public:
        AppInitEvent() = default;

        NS_EVENT_CLASS_TYPE(AppInit)
        NS_EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @brief Event fired when the application shuts down
     */
    class AppShutdownEvent : public Event
    {
    public:
        AppShutdownEvent() = default;

        NS_EVENT_CLASS_TYPE(AppShutdown)
        NS_EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @brief Event fired every frame update
     */
    class AppUpdateEvent : public Event
    {
    public:
        explicit AppUpdateEvent(f32 deltaTime)
            : m_DeltaTime(deltaTime)
        {
        }

        f32 GetDeltaTime() const { return m_DeltaTime; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "AppUpdateEvent: dt=" << m_DeltaTime;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(AppUpdate)
        NS_EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        f32 m_DeltaTime;
    };
}

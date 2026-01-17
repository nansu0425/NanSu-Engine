#pragma once

#include "Events/Event.h"
#include <sstream>

namespace NanSu
{
    /**
     * @brief Event fired when the window is requested to close
     */
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        NS_EVENT_CLASS_TYPE(WindowClose)
        NS_EVENT_CLASS_CATEGORY(EventCategoryWindow | EventCategoryApplication)
    };

    /**
     * @brief Event fired when the window is resized
     */
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32 width, uint32 height)
            : m_Width(width)
            , m_Height(height)
        {
        }

        uint32 GetWidth() const { return m_Width; }
        uint32 GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << "x" << m_Height;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(WindowResize)
        NS_EVENT_CLASS_CATEGORY(EventCategoryWindow | EventCategoryApplication)

    private:
        uint32 m_Width;
        uint32 m_Height;
    };

    /**
     * @brief Event fired when the window gains focus
     */
    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() = default;

        NS_EVENT_CLASS_TYPE(WindowFocus)
        NS_EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    /**
     * @brief Event fired when the window loses focus
     */
    class WindowLostFocusEvent : public Event
    {
    public:
        WindowLostFocusEvent() = default;

        NS_EVENT_CLASS_TYPE(WindowLostFocus)
        NS_EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };
}

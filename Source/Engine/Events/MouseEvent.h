#pragma once

#include "Events/Event.h"
#include <sstream>

namespace NanSu
{
    /**
     * @brief Event fired when the mouse is moved
     */
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float32 x, float32 y)
            : m_MouseX(x)
            , m_MouseY(y)
        {
        }

        float32 GetX() const { return m_MouseX; }
        float32 GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(MouseMoved)
        NS_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

    private:
        float32 m_MouseX;
        float32 m_MouseY;
    };

    /**
     * @brief Event fired when the mouse wheel is scrolled
     */
    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float32 xOffset, float32 yOffset)
            : m_XOffset(xOffset)
            , m_YOffset(yOffset)
        {
        }

        float32 GetXOffset() const { return m_XOffset; }
        float32 GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(MouseScrolled)
        NS_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

    private:
        float32 m_XOffset;
        float32 m_YOffset;
    };

    /**
     * @brief Base class for mouse button events
     */
    class MouseButtonEvent : public Event
    {
    public:
        int32 GetMouseButton() const { return m_Button; }

        NS_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

    protected:
        explicit MouseButtonEvent(int32 button)
            : m_Button(button)
        {
        }

        int32 m_Button;
    };

    /**
     * @brief Event fired when a mouse button is pressed
     */
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(int32 button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    /**
     * @brief Event fired when a mouse button is released
     */
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(int32 button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}

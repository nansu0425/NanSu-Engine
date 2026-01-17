#pragma once

#include "Events/Event.h"
#include <sstream>

namespace NanSu
{
    /**
     * @brief Base class for keyboard events
     */
    class KeyEvent : public Event
    {
    public:
        int32 GetKeyCode() const { return m_KeyCode; }

        NS_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

    protected:
        explicit KeyEvent(int32 keyCode)
            : m_KeyCode(keyCode)
        {
        }

        int32 m_KeyCode;
    };

    /**
     * @brief Event fired when a key is pressed
     */
    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int32 keyCode, bool isRepeat = false)
            : KeyEvent(keyCode)
            , m_IsRepeat(isRepeat)
        {
        }

        bool IsRepeat() const { return m_IsRepeat; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (repeat=" << m_IsRepeat << ")";
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool m_IsRepeat;
    };

    /**
     * @brief Event fired when a key is released
     */
    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(int32 keyCode)
            : KeyEvent(keyCode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(KeyReleased)
    };

    /**
     * @brief Event fired when a character is typed (for text input)
     */
    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(int32 keyCode)
            : KeyEvent(keyCode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        NS_EVENT_CLASS_TYPE(KeyTyped)
    };
}

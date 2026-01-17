#include "EnginePCH.h"
#include "Input/KeyCodes.h"

#ifdef NS_PLATFORM_WINDOWS

namespace NanSu
{
    // VK code to KeyCode lookup table
    static KeyCode s_VKToKeyCode[256];
    static bool s_MappingInitialized = false;

    static void InitializeKeyMapping()
    {
        if (s_MappingInitialized)
        {
            return;
        }

        // Initialize all to Unknown
        for (int32 i = 0; i < 256; ++i)
        {
            s_VKToKeyCode[i] = KeyCode::Unknown;
        }

        // Letters (A-Z share VK codes with ASCII)
        for (int32 i = 'A'; i <= 'Z'; ++i)
        {
            s_VKToKeyCode[i] = static_cast<KeyCode>(i);
        }

        // Numbers (0-9 share VK codes with ASCII)
        for (int32 i = '0'; i <= '9'; ++i)
        {
            s_VKToKeyCode[i] = static_cast<KeyCode>(i);
        }

        // Printable keys
        s_VKToKeyCode[VK_SPACE]      = KeyCode::Space;
        s_VKToKeyCode[VK_OEM_7]      = KeyCode::Apostrophe;     // '
        s_VKToKeyCode[VK_OEM_COMMA]  = KeyCode::Comma;          // ,
        s_VKToKeyCode[VK_OEM_MINUS]  = KeyCode::Minus;          // -
        s_VKToKeyCode[VK_OEM_PERIOD] = KeyCode::Period;         // .
        s_VKToKeyCode[VK_OEM_2]      = KeyCode::Slash;          // /
        s_VKToKeyCode[VK_OEM_1]      = KeyCode::Semicolon;      // ;
        s_VKToKeyCode[VK_OEM_PLUS]   = KeyCode::Equal;          // =
        s_VKToKeyCode[VK_OEM_4]      = KeyCode::LeftBracket;    // [
        s_VKToKeyCode[VK_OEM_5]      = KeyCode::Backslash;      // '\'
        s_VKToKeyCode[VK_OEM_6]      = KeyCode::RightBracket;   // ]
        s_VKToKeyCode[VK_OEM_3]      = KeyCode::GraveAccent;    // `

        // Function keys
        s_VKToKeyCode[VK_ESCAPE]     = KeyCode::Escape;
        s_VKToKeyCode[VK_RETURN]     = KeyCode::Enter;
        s_VKToKeyCode[VK_TAB]        = KeyCode::Tab;
        s_VKToKeyCode[VK_BACK]       = KeyCode::Backspace;
        s_VKToKeyCode[VK_INSERT]     = KeyCode::Insert;
        s_VKToKeyCode[VK_DELETE]     = KeyCode::Delete;
        s_VKToKeyCode[VK_RIGHT]      = KeyCode::Right;
        s_VKToKeyCode[VK_LEFT]       = KeyCode::Left;
        s_VKToKeyCode[VK_DOWN]       = KeyCode::Down;
        s_VKToKeyCode[VK_UP]         = KeyCode::Up;
        s_VKToKeyCode[VK_PRIOR]      = KeyCode::PageUp;
        s_VKToKeyCode[VK_NEXT]       = KeyCode::PageDown;
        s_VKToKeyCode[VK_HOME]       = KeyCode::Home;
        s_VKToKeyCode[VK_END]        = KeyCode::End;
        s_VKToKeyCode[VK_CAPITAL]    = KeyCode::CapsLock;
        s_VKToKeyCode[VK_SCROLL]     = KeyCode::ScrollLock;
        s_VKToKeyCode[VK_NUMLOCK]    = KeyCode::NumLock;
        s_VKToKeyCode[VK_SNAPSHOT]   = KeyCode::PrintScreen;
        s_VKToKeyCode[VK_PAUSE]      = KeyCode::Pause;

        // F-keys (F1-F24)
        for (int32 i = 0; i < 24; ++i)
        {
            s_VKToKeyCode[VK_F1 + i] = static_cast<KeyCode>(static_cast<int32>(KeyCode::F1) + i);
        }

        // Keypad numbers
        for (int32 i = 0; i < 10; ++i)
        {
            s_VKToKeyCode[VK_NUMPAD0 + i] = static_cast<KeyCode>(static_cast<int32>(KeyCode::Numpad0) + i);
        }

        // Keypad operators
        s_VKToKeyCode[VK_DECIMAL]    = KeyCode::NumpadDecimal;
        s_VKToKeyCode[VK_DIVIDE]     = KeyCode::NumpadDivide;
        s_VKToKeyCode[VK_MULTIPLY]   = KeyCode::NumpadMultiply;
        s_VKToKeyCode[VK_SUBTRACT]   = KeyCode::NumpadSubtract;
        s_VKToKeyCode[VK_ADD]        = KeyCode::NumpadAdd;

        // Modifiers (left/right specific)
        s_VKToKeyCode[VK_LSHIFT]     = KeyCode::LeftShift;
        s_VKToKeyCode[VK_RSHIFT]     = KeyCode::RightShift;
        s_VKToKeyCode[VK_LCONTROL]   = KeyCode::LeftControl;
        s_VKToKeyCode[VK_RCONTROL]   = KeyCode::RightControl;
        s_VKToKeyCode[VK_LMENU]      = KeyCode::LeftAlt;
        s_VKToKeyCode[VK_RMENU]      = KeyCode::RightAlt;
        s_VKToKeyCode[VK_LWIN]       = KeyCode::LeftSuper;
        s_VKToKeyCode[VK_RWIN]       = KeyCode::RightSuper;
        s_VKToKeyCode[VK_APPS]       = KeyCode::Menu;

        s_MappingInitialized = true;
    }

    KeyCode VirtualKeyToKeyCode(int32 vkCode, int32 scanCode, bool extended)
    {
        InitializeKeyMapping();

        if (vkCode < 0 || vkCode >= 256)
        {
            return KeyCode::Unknown;
        }

        // Handle left/right modifier disambiguation
        if (vkCode == VK_SHIFT)
        {
            UINT mappedVK = MapVirtualKey(static_cast<UINT>(scanCode), MAPVK_VSC_TO_VK_EX);
            return (mappedVK == VK_RSHIFT) ? KeyCode::RightShift : KeyCode::LeftShift;
        }

        if (vkCode == VK_CONTROL)
        {
            return extended ? KeyCode::RightControl : KeyCode::LeftControl;
        }

        if (vkCode == VK_MENU)
        {
            return extended ? KeyCode::RightAlt : KeyCode::LeftAlt;
        }

        return s_VKToKeyCode[vkCode];
    }

    int32 KeyCodeToVirtualKey(KeyCode keyCode)
    {
        switch (keyCode)
        {
            // Letters (A-Z have matching codes)
            case KeyCode::A: case KeyCode::B: case KeyCode::C: case KeyCode::D:
            case KeyCode::E: case KeyCode::F: case KeyCode::G: case KeyCode::H:
            case KeyCode::I: case KeyCode::J: case KeyCode::K: case KeyCode::L:
            case KeyCode::M: case KeyCode::N: case KeyCode::O: case KeyCode::P:
            case KeyCode::Q: case KeyCode::R: case KeyCode::S: case KeyCode::T:
            case KeyCode::U: case KeyCode::V: case KeyCode::W: case KeyCode::X:
            case KeyCode::Y: case KeyCode::Z:
                return static_cast<int32>(keyCode);

            // Numbers (0-9 have matching codes)
            case KeyCode::D0: case KeyCode::D1: case KeyCode::D2: case KeyCode::D3:
            case KeyCode::D4: case KeyCode::D5: case KeyCode::D6: case KeyCode::D7:
            case KeyCode::D8: case KeyCode::D9:
                return static_cast<int32>(keyCode);

            // Printable keys
            case KeyCode::Space:        return VK_SPACE;
            case KeyCode::Apostrophe:   return VK_OEM_7;
            case KeyCode::Comma:        return VK_OEM_COMMA;
            case KeyCode::Minus:        return VK_OEM_MINUS;
            case KeyCode::Period:       return VK_OEM_PERIOD;
            case KeyCode::Slash:        return VK_OEM_2;
            case KeyCode::Semicolon:    return VK_OEM_1;
            case KeyCode::Equal:        return VK_OEM_PLUS;
            case KeyCode::LeftBracket:  return VK_OEM_4;
            case KeyCode::Backslash:    return VK_OEM_5;
            case KeyCode::RightBracket: return VK_OEM_6;
            case KeyCode::GraveAccent:  return VK_OEM_3;

            // Function keys
            case KeyCode::Escape:       return VK_ESCAPE;
            case KeyCode::Enter:        return VK_RETURN;
            case KeyCode::Tab:          return VK_TAB;
            case KeyCode::Backspace:    return VK_BACK;
            case KeyCode::Insert:       return VK_INSERT;
            case KeyCode::Delete:       return VK_DELETE;
            case KeyCode::Right:        return VK_RIGHT;
            case KeyCode::Left:         return VK_LEFT;
            case KeyCode::Down:         return VK_DOWN;
            case KeyCode::Up:           return VK_UP;
            case KeyCode::PageUp:       return VK_PRIOR;
            case KeyCode::PageDown:     return VK_NEXT;
            case KeyCode::Home:         return VK_HOME;
            case KeyCode::End:          return VK_END;
            case KeyCode::CapsLock:     return VK_CAPITAL;
            case KeyCode::ScrollLock:   return VK_SCROLL;
            case KeyCode::NumLock:      return VK_NUMLOCK;
            case KeyCode::PrintScreen:  return VK_SNAPSHOT;
            case KeyCode::Pause:        return VK_PAUSE;

            // F-keys
            case KeyCode::F1:  return VK_F1;  case KeyCode::F2:  return VK_F2;
            case KeyCode::F3:  return VK_F3;  case KeyCode::F4:  return VK_F4;
            case KeyCode::F5:  return VK_F5;  case KeyCode::F6:  return VK_F6;
            case KeyCode::F7:  return VK_F7;  case KeyCode::F8:  return VK_F8;
            case KeyCode::F9:  return VK_F9;  case KeyCode::F10: return VK_F10;
            case KeyCode::F11: return VK_F11; case KeyCode::F12: return VK_F12;
            case KeyCode::F13: return VK_F13; case KeyCode::F14: return VK_F14;
            case KeyCode::F15: return VK_F15; case KeyCode::F16: return VK_F16;
            case KeyCode::F17: return VK_F17; case KeyCode::F18: return VK_F18;
            case KeyCode::F19: return VK_F19; case KeyCode::F20: return VK_F20;
            case KeyCode::F21: return VK_F21; case KeyCode::F22: return VK_F22;
            case KeyCode::F23: return VK_F23; case KeyCode::F24: return VK_F24;

            // Keypad
            case KeyCode::Numpad0: return VK_NUMPAD0;
            case KeyCode::Numpad1: return VK_NUMPAD1;
            case KeyCode::Numpad2: return VK_NUMPAD2;
            case KeyCode::Numpad3: return VK_NUMPAD3;
            case KeyCode::Numpad4: return VK_NUMPAD4;
            case KeyCode::Numpad5: return VK_NUMPAD5;
            case KeyCode::Numpad6: return VK_NUMPAD6;
            case KeyCode::Numpad7: return VK_NUMPAD7;
            case KeyCode::Numpad8: return VK_NUMPAD8;
            case KeyCode::Numpad9: return VK_NUMPAD9;
            case KeyCode::NumpadDecimal:  return VK_DECIMAL;
            case KeyCode::NumpadDivide:   return VK_DIVIDE;
            case KeyCode::NumpadMultiply: return VK_MULTIPLY;
            case KeyCode::NumpadSubtract: return VK_SUBTRACT;
            case KeyCode::NumpadAdd:      return VK_ADD;
            case KeyCode::NumpadEnter:    return VK_RETURN;
            case KeyCode::NumpadEqual:    return VK_OEM_NEC_EQUAL;

            // Modifiers
            case KeyCode::LeftShift:    return VK_LSHIFT;
            case KeyCode::RightShift:   return VK_RSHIFT;
            case KeyCode::LeftControl:  return VK_LCONTROL;
            case KeyCode::RightControl: return VK_RCONTROL;
            case KeyCode::LeftAlt:      return VK_LMENU;
            case KeyCode::RightAlt:     return VK_RMENU;
            case KeyCode::LeftSuper:    return VK_LWIN;
            case KeyCode::RightSuper:   return VK_RWIN;
            case KeyCode::Menu:         return VK_APPS;

            default: return 0;
        }
    }
}

#endif // NS_PLATFORM_WINDOWS

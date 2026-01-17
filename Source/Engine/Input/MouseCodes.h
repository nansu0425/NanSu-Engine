#pragma once

#include "Core/Types.h"

namespace NanSu
{
    /**
     * @brief Platform-independent mouse button codes
     */
    enum class MouseCode : int32
    {
        Button0         = 0,    // Left button
        Button1         = 1,    // Right button
        Button2         = 2,    // Middle button
        Button3         = 3,    // Extra button 1 (X1)
        Button4         = 4,    // Extra button 2 (X2)
        Button5         = 5,
        Button6         = 6,
        Button7         = 7,

        // Aliases for clarity
        Left            = Button0,
        Right           = Button1,
        Middle          = Button2
    };
}

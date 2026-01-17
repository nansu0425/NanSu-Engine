#pragma once

// =============================================================================
// NanSu Engine Precompiled Header
// =============================================================================

// -----------------------------------------------------------------------------
// Platform Detection
// -----------------------------------------------------------------------------
#ifdef NS_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <Windows.h>
#endif

// -----------------------------------------------------------------------------
// C++ Standard Library - Core
// -----------------------------------------------------------------------------
#include <string>
#include <memory>
#include <utility>
#include <functional>
#include <cstdint>

// -----------------------------------------------------------------------------
// C++ Standard Library - Containers
// -----------------------------------------------------------------------------
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

// -----------------------------------------------------------------------------
// C++ Standard Library - Algorithms
// -----------------------------------------------------------------------------
#include <algorithm>

// -----------------------------------------------------------------------------
// C++ Standard Library - I/O
// -----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <fstream>

// -----------------------------------------------------------------------------
// Third-Party - spdlog
// -----------------------------------------------------------------------------
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// -----------------------------------------------------------------------------
// Engine Core
// -----------------------------------------------------------------------------
#include "Core/Types.h"
#include "Core/Logger.h"
#include "Core/Assert.h"
#include "Core/Window.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"

// -----------------------------------------------------------------------------
// Engine Input
// -----------------------------------------------------------------------------
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

// -----------------------------------------------------------------------------
// Engine Events
// -----------------------------------------------------------------------------
#include "Events/Event.h"
#include "Events/EventDispatcher.h"

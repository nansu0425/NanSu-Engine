# NanSu Engine - Coding Standards

**Version:** 1.0.0  
**Last Updated:** 2026-01-17  
**Project Type:** C++ Game Engine + Lua Game Logic

---

## Table of Contents

1. [Overview](#overview)
2. [General Principles](#general-principles)
3. [C++ Coding Standards](#c-coding-standards)
4. [Lua Coding Standards](#lua-coding-standards)

---

## Overview

This document defines the coding conventions for the **NanSu Engine** project. The engine is built using:
- **C++20** for core engine and editor
- **Lua** for game logic scripting (future)
- **Premake5** for cross-platform build configuration
- Target platforms: Windows, Linux, macOS (future)

---

## General Principles

### Code Philosophy
- **Clarity over cleverness**: Write code that is easy to understand and maintain
- **Consistency**: Follow established patterns throughout the codebase
- **Performance-conscious**: Consider performance implications, but prioritize readability first
- **Cross-platform compatibility**: Avoid platform-specific code where possible

### Documentation
- Document complex algorithms and non-obvious design decisions
- Use self-documenting code (clear naming, small functions)
- Keep comments up-to-date with code changes
- Document public APIs thoroughly

---

## C++ Coding Standards

### 1. Naming Conventions

#### Namespaces
```cpp
namespace NanSu
{
    namespace Core
    {
        // ...
    }
}
```
- Use **PascalCase** for namespace names
- Keep namespace names concise and meaningful
- Primary namespace: `NanSu`

#### Classes and Structs
```cpp
class Engine { };
class RenderSystem { };
struct TransformComponent { };
```
- Use **PascalCase** for class and struct names
- Use descriptive, noun-based names
- Avoid prefixes like `C` or `T`

#### Member Variables
```cpp
class Engine
{
private:
    std::string m_Version;        // Member variable
    bool m_IsInitialized;         // Member variable
    static int s_InstanceCount;   // Static member variable
};
```
- Use **camelCase** with `m_` prefix for member variables
- Use `s_` prefix for static member variables
- Use `g_` prefix for global variables (avoid when possible)

#### Functions and Methods
```cpp
void Initialize();
void Shutdown();
std::string GetVersion() const;
bool IsRunning() const;
```
- Use **PascalCase** for function names
- Use verb-based names that describe the action
- Getters: `GetPropertyName()`
- Setters: `SetPropertyName()`
- Boolean queries: `IsCondition()`, `HasProperty()`, `CanPerformAction()`

#### Constants and Enums
```cpp
const int MAX_ENTITIES = 1000;
constexpr float PI = 3.14159f;

enum class RenderMode
{
    Wireframe,
    Solid,
    Textured
};
```
- Use **UPPER_SNAKE_CASE** for compile-time constants
- Use **enum class** (strongly-typed enums)
- Use **PascalCase** for enum names and values

#### Local Variables and Parameters
```cpp
void ProcessEntity(int entityId, const std::string& entityName)
{
    int localCounter = 0;
    float deltaTime = 0.016f;
}
```
- Use **camelCase** for local variables and function parameters

#### Template Parameters
```cpp
template<typename T, typename Allocator>
class Container { };
```
- Use **PascalCase** for template type parameters
- Prefer `typename` over `class` keyword

### 2. Code Formatting

#### Indentation
- Use **4 spaces** for indentation (no tabs)
- Configure your editor to convert tabs to spaces

#### Braces
```cpp
// Good: Braces on new line (Allman style)
void Function()
{
    if (condition)
    {
        DoSomething();
    }
    else
    {
        DoSomethingElse();
    }
}

// For single-line statements, braces are optional but recommended
if (condition)
{
    DoSomething();
}
```

#### Line Length
- Maximum **120 characters** per line
- Break long lines logically

#### Spacing
```cpp
// Operators
int result = a + b * c;
bool check = (x > 5) && (y < 10);

// Function calls
Function(arg1, arg2, arg3);

// Control structures
if (condition)
for (int i = 0; i < count; ++i)
while (running)
```

### 3. Header Files

#### Include Guards
```cpp
#pragma once

// Header content
```
- Use `#pragma once` instead of traditional include guards
- It's simpler and supported by all modern compilers

#### Include Order
```cpp
// 1. Related header (for .cpp files)
#include "Core/Engine.h"

// 2. C system headers
#include <stdint.h>

// 3. C++ standard library headers
#include <string>
#include <vector>
#include <iostream>

// 4. Third-party library headers
// #include <GLFW/glfw3.h>

// 5. Project headers
#include "Core/Logger.h"
#include "Math/Vector3.h"
```

#### Forward Declarations
```cpp
// Use forward declarations to reduce dependencies
namespace NanSu
{
    class Renderer;  // Forward declaration
    class World;     // Forward declaration
    
    class Engine
    {
    private:
        Renderer* m_Renderer;
        World* m_World;
    };
}
```

### 4. Classes and Objects

#### Constructor Initialization Lists
```cpp
// Good: Use initialization lists
Engine::Engine()
    : m_Version("1.0.0")
    , m_IsInitialized(false)
    , m_FrameCount(0)
{
    // Constructor body
}
```

#### Rule of Five
```cpp
class Resource
{
public:
    Resource();                                    // Constructor
    ~Resource();                                   // Destructor
    Resource(const Resource& other);               // Copy constructor
    Resource& operator=(const Resource& other);    // Copy assignment
    Resource(Resource&& other) noexcept;           // Move constructor
    Resource& operator=(Resource&& other) noexcept; // Move assignment
};

// Or use = default / = delete
class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
```

#### Const Correctness
```cpp
class Engine
{
public:
    // Const methods don't modify object state
    std::string GetVersion() const;
    void PrintInfo() const;
    
    // Non-const methods can modify state
    void Initialize();
    void Shutdown();
    
    // Const parameters
    void SetName(const std::string& name);
};
```

### 5. Modern C++ Features (C++20)

#### Use Smart Pointers
```cpp
#include <memory>

// Prefer smart pointers over raw pointers
std::unique_ptr<Renderer> m_Renderer;
std::shared_ptr<World> m_World;

// Use make_unique and make_shared
m_Renderer = std::make_unique<Renderer>();
m_World = std::make_shared<World>();
```

#### Use auto Appropriately
```cpp
// Good: When type is obvious or verbose
auto renderer = std::make_unique<Renderer>();
auto it = container.begin();

// Avoid: When type is not immediately clear
auto value = GetValue();  // What type is this?
```

#### Range-Based For Loops
```cpp
std::vector<Entity> entities;

// Use range-based for loops
for (const auto& entity : entities)
{
    entity.Update();
}
```

#### nullptr Instead of NULL
```cpp
// Good
void* ptr = nullptr;

// Avoid
void* ptr = NULL;
void* ptr = 0;
```

### 6. Comments

#### Single-Line Comments
```cpp
// Use // for single-line comments
int frameCount = 0;  // Comment after code
```

#### Multi-Line Comments
```cpp
/*
 * Use block comments for multi-line explanations
 * when documenting complex logic or algorithms
 */
```

#### Documentation Comments
```cpp
/**
 * @brief Initializes the engine subsystems
 * 
 * This method sets up all core engine components including
 * the renderer, audio system, and input manager.
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool Initialize();
```

### 7. Error Handling

```cpp
// Use exceptions for exceptional cases
void LoadResource(const std::string& path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Path cannot be empty");
    }
    
    // Load resource...
}

// Use return codes for expected failures
bool TryLoadResource(const std::string& path)
{
    if (!FileExists(path))
    {
        return false;
    }
    
    // Load resource...
    return true;
}
```

### 8. Platform-Specific Code

```cpp
// Use preprocessor directives for platform-specific code
#ifdef NS_PLATFORM_WINDOWS
    #include <Windows.h>
    // Windows-specific code
#elif defined(NS_PLATFORM_LINUX)
    #include <unistd.h>
    // Linux-specific code
#elif defined(NS_PLATFORM_MACOS)
    // macOS-specific code
#endif

// Prefer abstracting platform-specific code into separate files
// e.g., PlatformWindows.cpp, PlatformLinux.cpp
```

---

## Lua Coding Standards

### 1. Naming Conventions

#### Variables and Functions
```lua
-- Use snake_case for variables and functions
local player_health = 100
local max_speed = 10.0

function update_player(delta_time)
    -- Function body
end
```

#### Constants
```lua
-- Use UPPER_SNAKE_CASE for constants
local MAX_PLAYERS = 4
local DEFAULT_GRAVITY = -9.81
```

#### Classes/Tables
```lua
-- Use PascalCase for class/module names
Player = {}
Player.__index = Player

function Player:new()
    local instance = setmetatable({}, Player)
    instance.health = 100
    return instance
end
```

### 2. Code Formatting

#### Indentation
- Use **4 spaces** for indentation

#### Local Variables
```lua
-- Always prefer local variables
local x = 10
local y = 20

-- Avoid global variables unless necessary
-- GLOBAL_CONFIG = {}  -- Only when needed
```

#### Table Construction
```lua
-- Single line for short tables
local point = { x = 0, y = 0 }

-- Multi-line for complex tables
local player = {
    name = "Player1",
    health = 100,
    position = { x = 0, y = 0, z = 0 },
    inventory = {}
}
```

### 3. Comments

```lua
-- Single-line comment

--[[
    Multi-line comment
    for detailed explanations
]]

--- Documentation comment for LuaDoc
--- @param delta_time number Time since last frame
--- @return boolean Success status
function update(delta_time)
    return true
end
```

---


**End of Document**

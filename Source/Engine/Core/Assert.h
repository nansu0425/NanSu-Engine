#pragma once

#include "Core/Logger.h"

// =============================================================================
// Platform-specific debug break
// =============================================================================
#ifdef NS_PLATFORM_WINDOWS
    #define NS_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define NS_DEBUGBREAK() __builtin_trap()
#else
    #define NS_DEBUGBREAK() ((void)0)
#endif

// =============================================================================
// Assertion Macros
// =============================================================================

#ifdef NS_DEBUG

    // Engine-side assertions (for engine code)
    #define NS_ENGINE_ASSERT(condition, ...) \
        do { \
            if (!(condition)) \
            { \
                NS_ENGINE_CRITICAL("Assertion Failed: {}", #condition); \
                NS_ENGINE_CRITICAL(__VA_ARGS__); \
                NS_DEBUGBREAK(); \
            } \
        } while (false)

    #define NS_ENGINE_ASSERT_MSG(condition, message) \
        do { \
            if (!(condition)) \
            { \
                NS_ENGINE_CRITICAL("Assertion Failed: {}", #condition); \
                NS_ENGINE_CRITICAL("Message: {}", message); \
                NS_DEBUGBREAK(); \
            } \
        } while (false)

    // Client-side assertions (for game/editor code)
    #define NS_ASSERT(condition, ...) \
        do { \
            if (!(condition)) \
            { \
                NS_CRITICAL("Assertion Failed: {}", #condition); \
                NS_CRITICAL(__VA_ARGS__); \
                NS_DEBUGBREAK(); \
            } \
        } while (false)

    #define NS_ASSERT_MSG(condition, message) \
        do { \
            if (!(condition)) \
            { \
                NS_CRITICAL("Assertion Failed: {}", #condition); \
                NS_CRITICAL("Message: {}", message); \
                NS_DEBUGBREAK(); \
            } \
        } while (false)

#else
    // Release builds: assertions are no-ops
    #define NS_ENGINE_ASSERT(condition, ...) ((void)0)
    #define NS_ENGINE_ASSERT_MSG(condition, message) ((void)0)
    #define NS_ASSERT(condition, ...) ((void)0)
    #define NS_ASSERT_MSG(condition, message) ((void)0)
#endif

// =============================================================================
// Verify Macros (always evaluated, logs in debug, no-op logging in release)
// =============================================================================

#ifdef NS_DEBUG
    #define NS_VERIFY(condition, ...) \
        do { \
            if (!(condition)) \
            { \
                NS_ENGINE_ERROR("Verification Failed: {}", #condition); \
                NS_ENGINE_ERROR(__VA_ARGS__); \
            } \
        } while (false)
#else
    #define NS_VERIFY(condition, ...) \
        do { \
            (void)(condition); \
        } while (false)
#endif

// =============================================================================
// Static Assert (compile-time)
// =============================================================================
#define NS_STATIC_ASSERT(condition, message) static_assert(condition, message)

// =============================================================================
// Unreachable Code Marker
// =============================================================================
#ifdef NS_DEBUG
    #define NS_UNREACHABLE() \
        do { \
            NS_ENGINE_CRITICAL("Unreachable code executed at {}:{}", __FILE__, __LINE__); \
            NS_DEBUGBREAK(); \
        } while (false)
#else
    #ifdef _MSC_VER
        #define NS_UNREACHABLE() __assume(0)
    #elif defined(__GNUC__) || defined(__clang__)
        #define NS_UNREACHABLE() __builtin_unreachable()
    #else
        #define NS_UNREACHABLE() ((void)0)
    #endif
#endif

// =============================================================================
// Not Implemented Marker
// =============================================================================
#define NS_NOT_IMPLEMENTED() \
    do { \
        NS_ENGINE_WARN("Not implemented: {} at {}:{}", __FUNCTION__, __FILE__, __LINE__); \
        NS_DEBUGBREAK(); \
    } while (false)

// =============================================================================
// Deprecation Warning
// =============================================================================
#ifdef _MSC_VER
    #define NS_DEPRECATED(message) __declspec(deprecated(message))
#elif defined(__GNUC__) || defined(__clang__)
    #define NS_DEPRECATED(message) __attribute__((deprecated(message)))
#else
    #define NS_DEPRECATED(message)
#endif

// =============================================================================
// Function Signature (for logging)
// =============================================================================
#ifdef _MSC_VER
    #define NS_FUNC_SIG __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
    #define NS_FUNC_SIG __PRETTY_FUNCTION__
#else
    #define NS_FUNC_SIG __func__
#endif

// =============================================================================
// Debug-only Code Block
// =============================================================================
#ifdef NS_DEBUG
    #define NS_DEBUG_ONLY(code) code
#else
    #define NS_DEBUG_ONLY(code)
#endif

// =============================================================================
// Stringify Helpers
// =============================================================================
#define NS_STRINGIFY(x) #x
#define NS_STRINGIFY_MACRO(x) NS_STRINGIFY(x)

// =============================================================================
// Concatenation Helpers
// =============================================================================
#define NS_CONCAT_IMPL(a, b) a##b
#define NS_CONCAT(a, b) NS_CONCAT_IMPL(a, b)

// =============================================================================
// Unique Variable Name Generator
// =============================================================================
#define NS_UNIQUE_VAR(prefix) NS_CONCAT(prefix, __LINE__)

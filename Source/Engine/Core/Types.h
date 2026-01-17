#pragma once

#include <cstdint>
#include <cstddef>

namespace NanSu
{
    // =========================================================================
    // Signed Integer Types
    // =========================================================================
    using int8  = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;

    // =========================================================================
    // Unsigned Integer Types
    // =========================================================================
    using uint8  = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;

    // =========================================================================
    // Floating Point Types
    // =========================================================================
    using f32 = float;
    using f64 = double;

    // =========================================================================
    // Size Types
    // =========================================================================
    using usize = std::size_t;
    using isize = std::ptrdiff_t;

    // =========================================================================
    // Byte Type
    // =========================================================================
    using byte = uint8;

    // =========================================================================
    // Pointer Types
    // =========================================================================
    using uintptr = std::uintptr_t;
    using intptr  = std::intptr_t;

    // =========================================================================
    // Graphics Handle Types (Forward declarations for future use)
    // =========================================================================
    // These will be defined based on the active graphics API
    // DX11: ID3D11Device*, ID3D11DeviceContext*, etc.
    // DX12: ID3D12Device*, ID3D12CommandQueue*, etc.
    // Vulkan: VkDevice, VkInstance, etc.

    // Opaque handle type for graphics resources
    using GraphicsHandle = void*;

    // =========================================================================
    // Common Type Limits
    // =========================================================================
    namespace Limits
    {
        inline constexpr int8  Int8Min   = INT8_MIN;
        inline constexpr int8  Int8Max   = INT8_MAX;
        inline constexpr int16 Int16Min  = INT16_MIN;
        inline constexpr int16 Int16Max  = INT16_MAX;
        inline constexpr int32 Int32Min  = INT32_MIN;
        inline constexpr int32 Int32Max  = INT32_MAX;
        inline constexpr int64 Int64Min  = INT64_MIN;
        inline constexpr int64 Int64Max  = INT64_MAX;

        inline constexpr uint8  UInt8Max  = UINT8_MAX;
        inline constexpr uint16 UInt16Max = UINT16_MAX;
        inline constexpr uint32 UInt32Max = UINT32_MAX;
        inline constexpr uint64 UInt64Max = UINT64_MAX;
    }
}

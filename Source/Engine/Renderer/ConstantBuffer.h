#pragma once

#include "Core/Types.h"

namespace NanSu
{
    /**
     * @brief Abstract interface for GPU constant buffers
     *
     * Constant buffers upload CPU data to the GPU for use in shaders.
     * Used for passing transformation matrices, material properties, etc.
     *
     * Use ConstantBuffer::Create() factory method for platform-specific creation.
     *
     * Example usage:
     * @code
     * struct SceneData { mat4 ViewProjection; };
     * auto* cbuffer = ConstantBuffer::Create(sizeof(SceneData));
     * cbuffer->SetData(&sceneData, sizeof(SceneData));
     * cbuffer->Bind(0);  // Bind to slot b0
     * @endcode
     */
    class ConstantBuffer
    {
    public:
        virtual ~ConstantBuffer() = default;

        // Non-copyable
        ConstantBuffer(const ConstantBuffer&) = delete;
        ConstantBuffer& operator=(const ConstantBuffer&) = delete;

        /**
         * @brief Upload data to the constant buffer
         * @param data Pointer to source data
         * @param size Size of data in bytes
         */
        virtual void SetData(const void* data, uint32 size) = 0;

        /**
         * @brief Bind this constant buffer to a shader slot
         * @param slot The constant buffer slot (0 = b0, 1 = b1, etc.)
         */
        virtual void Bind(uint32 slot) const = 0;

        /**
         * @brief Unbind this constant buffer from a shader slot
         * @param slot The slot to unbind
         */
        virtual void Unbind(uint32 slot) const = 0;

        /**
         * @brief Create a constant buffer with initial size
         * @param size Size in bytes (must be multiple of 16 for DX11)
         * @return Pointer to created buffer (caller owns memory)
         */
        static ConstantBuffer* Create(uint32 size);

    protected:
        ConstantBuffer() = default;
    };

}

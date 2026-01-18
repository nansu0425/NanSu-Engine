#pragma once

#include "Renderer/Buffer.h"

#ifdef NS_PLATFORM_WINDOWS

// Forward declarations to avoid including DX11 headers
struct ID3D11Buffer;

namespace NanSu
{
    /**
     * @brief DirectX 11 implementation of VertexBuffer
     */
    class DX11VertexBuffer : public VertexBuffer
    {
    public:
        /**
         * @brief Create a static vertex buffer with the given data (immutable)
         * @param vertices Pointer to vertex data
         * @param size Size of vertex data in bytes
         */
        DX11VertexBuffer(const void* vertices, uint32 size);

        /**
         * @brief Create a dynamic vertex buffer that can be updated
         * @param size Maximum size of buffer in bytes
         */
        DX11VertexBuffer(uint32 size);

        ~DX11VertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        const BufferLayout& GetLayout() const override { return m_Layout; }

        void SetData(const void* data, uint32 size) override;

    private:
        ID3D11Buffer* m_Buffer = nullptr;
        BufferLayout m_Layout;
        uint32 m_Size = 0;
        bool m_IsDynamic = false;
    };

    /**
     * @brief DirectX 11 implementation of IndexBuffer
     */
    class DX11IndexBuffer : public IndexBuffer
    {
    public:
        /**
         * @brief Create an index buffer with the given data
         * @param indices Pointer to index data
         * @param count Number of indices
         */
        DX11IndexBuffer(const uint32* indices, uint32 count);
        ~DX11IndexBuffer();

        void Bind() const override;
        void Unbind() const override;
        uint32 GetCount() const override { return m_Count; }

    private:
        ID3D11Buffer* m_Buffer = nullptr;
        uint32 m_Count = 0;
    };

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

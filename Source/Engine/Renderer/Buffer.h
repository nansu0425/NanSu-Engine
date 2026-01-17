#pragma once

#include "Core/Types.h"

#include <string>
#include <vector>

namespace NanSu
{
    // =========================================================================
    // ShaderDataType
    // =========================================================================

    /**
     * @brief Shader data types for buffer layout specification
     */
    enum class ShaderDataType : uint8
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat3, Mat4,
        Bool
    };

    /**
     * @brief Get the size in bytes for a shader data type
     * @param type The shader data type
     * @return Size in bytes
     */
    inline uint32 ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:     return 4;
            case ShaderDataType::Float2:    return 4 * 2;
            case ShaderDataType::Float3:    return 4 * 3;
            case ShaderDataType::Float4:    return 4 * 4;
            case ShaderDataType::Int:       return 4;
            case ShaderDataType::Int2:      return 4 * 2;
            case ShaderDataType::Int3:      return 4 * 3;
            case ShaderDataType::Int4:      return 4 * 4;
            case ShaderDataType::Mat3:      return 4 * 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4 * 4;
            case ShaderDataType::Bool:      return 1;
            case ShaderDataType::None:      return 0;
        }

        NS_ENGINE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }

    // =========================================================================
    // BufferElement
    // =========================================================================

    /**
     * @brief Describes a single element in a buffer layout
     */
    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32 Size;
        uint32 Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name)
            , Type(type)
            , Size(ShaderDataTypeSize(type))
            , Offset(0)
            , Normalized(normalized)
        {
        }

        /**
         * @brief Get the number of components in this element
         * @return Component count (e.g., Float3 returns 3)
         */
        uint32 GetComponentCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Float:     return 1;
                case ShaderDataType::Float2:    return 2;
                case ShaderDataType::Float3:    return 3;
                case ShaderDataType::Float4:    return 4;
                case ShaderDataType::Int:       return 1;
                case ShaderDataType::Int2:      return 2;
                case ShaderDataType::Int3:      return 3;
                case ShaderDataType::Int4:      return 4;
                case ShaderDataType::Mat3:      return 3 * 3;
                case ShaderDataType::Mat4:      return 4 * 4;
                case ShaderDataType::Bool:      return 1;
                case ShaderDataType::None:      return 0;
            }

            NS_ENGINE_ASSERT(false, "Unknown ShaderDataType");
            return 0;
        }
    };

    // =========================================================================
    // BufferLayout
    // =========================================================================

    /**
     * @brief Describes the layout of vertex data in a buffer
     *
     * Example usage:
     * @code
     * BufferLayout layout = {
     *     { ShaderDataType::Float3, "Position" },
     *     { ShaderDataType::Float4, "Color" },
     *     { ShaderDataType::Float2, "TexCoord" }
     * };
     * @endcode
     */
    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32 GetStride() const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride()
        {
            uint32 offset = 0;
            m_Stride = 0;

            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32 m_Stride = 0;
    };

    // =========================================================================
    // VertexBuffer
    // =========================================================================

    /**
     * @brief Abstract interface for vertex buffers
     *
     * Use VertexBuffer::Create() factory method to create a platform-specific buffer.
     *
     * Example usage:
     * @code
     * f32 vertices[] = { -0.5f, -0.5f, 0.0f,  0.5f, -0.5f, 0.0f,  0.0f, 0.5f, 0.0f };
     * auto* vb = VertexBuffer::Create(vertices, sizeof(vertices));
     * vb->SetLayout({
     *     { ShaderDataType::Float3, "Position" }
     * });
     * @endcode
     */
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        // Non-copyable
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        /**
         * @brief Bind this buffer for rendering
         */
        virtual void Bind() const = 0;

        /**
         * @brief Unbind this buffer
         */
        virtual void Unbind() const = 0;

        /**
         * @brief Set the layout of vertex data in this buffer
         * @param layout The buffer layout describing vertex attributes
         */
        virtual void SetLayout(const BufferLayout& layout) = 0;

        /**
         * @brief Get the current buffer layout
         * @return The buffer layout
         */
        virtual const BufferLayout& GetLayout() const = 0;

        /**
         * @brief Create a vertex buffer with the given data
         * @param vertices Pointer to vertex data
         * @param size Size of vertex data in bytes
         * @return Pointer to the created buffer (caller owns memory)
         */
        static VertexBuffer* Create(const void* vertices, uint32 size);

    protected:
        VertexBuffer() = default;
    };

    // =========================================================================
    // IndexBuffer
    // =========================================================================

    /**
     * @brief Abstract interface for index buffers
     *
     * Use IndexBuffer::Create() factory method to create a platform-specific buffer.
     *
     * Example usage:
     * @code
     * uint32 indices[] = { 0, 1, 2 };
     * auto* ib = IndexBuffer::Create(indices, 3);
     * @endcode
     */
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        // Non-copyable
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        /**
         * @brief Bind this buffer for rendering
         */
        virtual void Bind() const = 0;

        /**
         * @brief Unbind this buffer
         */
        virtual void Unbind() const = 0;

        /**
         * @brief Get the number of indices in this buffer
         * @return Index count
         */
        virtual uint32 GetCount() const = 0;

        /**
         * @brief Create an index buffer with the given data
         * @param indices Pointer to index data
         * @param count Number of indices
         * @return Pointer to the created buffer (caller owns memory)
         */
        static IndexBuffer* Create(const uint32* indices, uint32 count);

    protected:
        IndexBuffer() = default;
    };

} // namespace NanSu

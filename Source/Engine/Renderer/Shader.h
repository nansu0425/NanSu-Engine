#pragma once

#include "Core/Types.h"
#include "Renderer/Buffer.h"

#include <string>

namespace NanSu
{
    /**
     * @brief Abstract shader interface for platform-independent shader management
     *
     * Use Shader::Create() factory methods to create platform-specific shaders.
     *
     * Example usage:
     * @code
     * auto* shader = Shader::Create("Assets/Shaders/Basic.hlsl");
     * shader->SetInputLayout(layout);
     * shader->Bind();
     * // ... render geometry
     * shader->Unbind();
     * @endcode
     */
    class Shader
    {
    public:
        virtual ~Shader() = default;

        // Non-copyable
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        /**
         * @brief Bind this shader for rendering
         * Sets vertex shader, pixel shader, and input layout
         */
        virtual void Bind() const = 0;

        /**
         * @brief Unbind this shader
         */
        virtual void Unbind() const = 0;

        /**
         * @brief Set the input layout for this shader
         * @param layout The buffer layout describing vertex attributes
         *
         * Must be called before Bind() if using vertex buffers
         */
        virtual void SetInputLayout(const BufferLayout& layout) = 0;

        /**
         * @brief Get the shader name (typically the filename without extension)
         * @return The shader name
         */
        virtual const std::string& GetName() const = 0;

        /**
         * @brief Create a shader from a single HLSL file containing VS and PS
         * @param filePath Path to the HLSL file (e.g., "Assets/Shaders/Basic.hlsl")
         * @return Pointer to the created shader (caller owns memory)
         *
         * The file should contain both VSMain and PSMain entry points
         */
        static Shader* Create(const std::string& filePath);

        /**
         * @brief Create a shader from separate vertex and pixel shader source strings
         * @param name Name identifier for this shader
         * @param vertexSource HLSL source code for vertex shader
         * @param pixelSource HLSL source code for pixel shader
         * @return Pointer to the created shader (caller owns memory)
         */
        static Shader* Create(const std::string& name,
                              const std::string& vertexSource,
                              const std::string& pixelSource);

    protected:
        Shader() = default;
    };

} // namespace NanSu

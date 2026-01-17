#pragma once

#include "Renderer/Shader.h"

#ifdef NS_PLATFORM_WINDOWS

// Forward declarations to avoid including DX11 headers
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace NanSu
{
    /**
     * @brief DirectX 11 implementation of Shader
     */
    class DX11Shader : public Shader
    {
    public:
        /**
         * @brief Create shader from a file path
         * @param filePath Path to HLSL file containing VSMain and PSMain
         */
        DX11Shader(const std::string& filePath);

        /**
         * @brief Create shader from source strings
         * @param name Shader name identifier
         * @param vertexSource HLSL vertex shader source
         * @param pixelSource HLSL pixel shader source
         */
        DX11Shader(const std::string& name,
                   const std::string& vertexSource,
                   const std::string& pixelSource);

        ~DX11Shader();

        void Bind() const override;
        void Unbind() const override;
        void SetInputLayout(const BufferLayout& layout) override;
        const std::string& GetName() const override { return m_Name; }

    private:
        /**
         * @brief Read shader file from disk
         * @param filePath Path to the shader file
         * @return File contents as string
         */
        std::string ReadFile(const std::string& filePath);

        /**
         * @brief Compile HLSL source to bytecode
         * @param source HLSL source code
         * @param entryPoint Entry point function name (e.g., "VSMain")
         * @param target Shader model target (e.g., "vs_5_0")
         * @return Compiled shader blob (ID3DBlob*)
         */
        void* CompileShader(const std::string& source,
                            const char* entryPoint,
                            const char* target);

        /**
         * @brief Create input layout from BufferLayout and VS bytecode
         * @param layout Vertex buffer layout
         */
        void CreateInputLayout(const BufferLayout& layout);

    private:
        std::string m_Name;
        std::string m_FilePath;

        ID3D11VertexShader* m_VertexShader = nullptr;
        ID3D11PixelShader* m_PixelShader = nullptr;
        ID3D11InputLayout* m_InputLayout = nullptr;

        // Keep VS bytecode for creating input layouts (ID3DBlob*)
        void* m_VSBlob = nullptr;
    };

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

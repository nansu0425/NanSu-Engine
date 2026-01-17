#include "EnginePCH.h"
#include "Platform/Windows/DX11Shader.h"

#ifdef NS_PLATFORM_WINDOWS

#include "Core/Application.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include <fstream>
#include <sstream>
#include <algorithm>

#pragma comment(lib, "d3dcompiler.lib")

namespace NanSu
{
    // =========================================================================
    // Helper Functions
    // =========================================================================

    namespace
    {
        DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type)
        {
            switch (type)
            {
                case ShaderDataType::Float:     return DXGI_FORMAT_R32_FLOAT;
                case ShaderDataType::Float2:    return DXGI_FORMAT_R32G32_FLOAT;
                case ShaderDataType::Float3:    return DXGI_FORMAT_R32G32B32_FLOAT;
                case ShaderDataType::Float4:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
                case ShaderDataType::Int:       return DXGI_FORMAT_R32_SINT;
                case ShaderDataType::Int2:      return DXGI_FORMAT_R32G32_SINT;
                case ShaderDataType::Int3:      return DXGI_FORMAT_R32G32B32_SINT;
                case ShaderDataType::Int4:      return DXGI_FORMAT_R32G32B32A32_SINT;
                case ShaderDataType::Bool:      return DXGI_FORMAT_R8_UINT;
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                case ShaderDataType::None:
                    break;
            }

            NS_ENGINE_ASSERT(false, "Unsupported ShaderDataType for input layout");
            return DXGI_FORMAT_UNKNOWN;
        }

        std::string ExtractNameFromPath(const std::string& filePath)
        {
            // Find last slash or backslash
            auto lastSlash = filePath.find_last_of("/\\");
            auto start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;

            // Find last dot
            auto lastDot = filePath.rfind('.');
            auto end = (lastDot == std::string::npos || lastDot < start)
                ? filePath.size()
                : lastDot;

            return filePath.substr(start, end - start);
        }
    }

    // =========================================================================
    // Factory Methods
    // =========================================================================

    Shader* Shader::Create(const std::string& filePath)
    {
        return new DX11Shader(filePath);
    }

    Shader* Shader::Create(const std::string& name,
                           const std::string& vertexSource,
                           const std::string& pixelSource)
    {
        return new DX11Shader(name, vertexSource, pixelSource);
    }

    // =========================================================================
    // DX11Shader
    // =========================================================================

    DX11Shader::DX11Shader(const std::string& filePath)
        : m_FilePath(filePath)
        , m_Name(ExtractNameFromPath(filePath))
    {
        std::string source = ReadFile(filePath);

        // Compile vertex shader
        m_VSBlob = CompileShader(source, "VSMain", "vs_5_0");
        NS_ENGINE_ASSERT(m_VSBlob, "Failed to compile vertex shader");

        // Compile pixel shader
        auto* psBlob = static_cast<ID3DBlob*>(CompileShader(source, "PSMain", "ps_5_0"));
        NS_ENGINE_ASSERT(psBlob, "Failed to compile pixel shader");

        // Create shaders
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        auto* vsBlob = static_cast<ID3DBlob*>(m_VSBlob);
        HRESULT hr = device->CreateVertexShader(
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr,
            &m_VertexShader
        );
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

        hr = device->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &m_PixelShader
        );
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");

        // Release PS blob (VS blob kept for input layout creation)
        psBlob->Release();

        NS_ENGINE_INFO("Shader '{}' created from file: {}", m_Name, filePath);
    }

    DX11Shader::DX11Shader(const std::string& name,
                           const std::string& vertexSource,
                           const std::string& pixelSource)
        : m_Name(name)
    {
        // Compile vertex shader
        m_VSBlob = CompileShader(vertexSource, "VSMain", "vs_5_0");
        NS_ENGINE_ASSERT(m_VSBlob, "Failed to compile vertex shader");

        // Compile pixel shader
        auto* psBlob = static_cast<ID3DBlob*>(CompileShader(pixelSource, "PSMain", "ps_5_0"));
        NS_ENGINE_ASSERT(psBlob, "Failed to compile pixel shader");

        // Create shaders
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        auto* vsBlob = static_cast<ID3DBlob*>(m_VSBlob);
        HRESULT hr = device->CreateVertexShader(
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr,
            &m_VertexShader
        );
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

        hr = device->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &m_PixelShader
        );
        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");

        // Release PS blob (VS blob kept for input layout creation)
        psBlob->Release();

        NS_ENGINE_INFO("Shader '{}' created from source", m_Name);
    }

    DX11Shader::~DX11Shader()
    {
        if (m_InputLayout)
        {
            m_InputLayout->Release();
            m_InputLayout = nullptr;
        }

        if (m_PixelShader)
        {
            m_PixelShader->Release();
            m_PixelShader = nullptr;
        }

        if (m_VertexShader)
        {
            m_VertexShader->Release();
            m_VertexShader = nullptr;
        }

        if (m_VSBlob)
        {
            static_cast<ID3DBlob*>(m_VSBlob)->Release();
            m_VSBlob = nullptr;
        }
    }

    void DX11Shader::Bind() const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->VSSetShader(m_VertexShader, nullptr, 0);
        deviceContext->PSSetShader(m_PixelShader, nullptr, 0);

        if (m_InputLayout)
        {
            deviceContext->IASetInputLayout(m_InputLayout);
        }
    }

    void DX11Shader::Unbind() const
    {
        auto* deviceContext = static_cast<ID3D11DeviceContext*>(
            Application::Get().GetGraphicsContext().GetNativeDeviceContext());

        deviceContext->VSSetShader(nullptr, nullptr, 0);
        deviceContext->PSSetShader(nullptr, nullptr, 0);
        deviceContext->IASetInputLayout(nullptr);
    }

    void DX11Shader::SetInputLayout(const BufferLayout& layout)
    {
        CreateInputLayout(layout);
    }

    std::string DX11Shader::ReadFile(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (!file)
        {
            NS_ENGINE_ERROR("Failed to open shader file: {}", filePath);
            NS_ENGINE_ASSERT(false, "Shader file not found");
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void* DX11Shader::CompileShader(const std::string& source,
                                     const char* entryPoint,
                                     const char* target)
    {
        UINT compileFlags = 0;
#ifdef NS_DEBUG
        compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompile(
            source.c_str(),
            source.size(),
            m_FilePath.empty() ? nullptr : m_FilePath.c_str(),
            nullptr,    // Defines
            nullptr,    // Include handler
            entryPoint,
            target,
            compileFlags,
            0,          // Effect compilation flags
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                NS_ENGINE_ERROR("Shader compilation failed for '{}' entry point '{}':",
                                m_Name, entryPoint);
                NS_ENGINE_ERROR("{}", static_cast<const char*>(errorBlob->GetBufferPointer()));
                errorBlob->Release();
            }
            return nullptr;
        }

        if (errorBlob)
        {
            // Warnings
            NS_ENGINE_WARN("Shader compilation warnings for '{}' entry point '{}':",
                           m_Name, entryPoint);
            NS_ENGINE_WARN("{}", static_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }

        return shaderBlob;
    }

    void DX11Shader::CreateInputLayout(const BufferLayout& layout)
    {
        auto* device = static_cast<ID3D11Device*>(
            Application::Get().GetGraphicsContext().GetNativeDevice());

        // Release existing input layout if any
        if (m_InputLayout)
        {
            m_InputLayout->Release();
            m_InputLayout = nullptr;
        }

        const auto& elements = layout.GetElements();

        // Store semantic names to ensure lifetime during CreateInputLayout call
        std::vector<std::string> semanticNames;
        semanticNames.reserve(elements.size());

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
        inputElements.reserve(elements.size());

        for (const auto& element : elements)
        {
            // Convert name to uppercase for HLSL semantic
            std::string semanticName = element.Name;
            std::transform(semanticName.begin(), semanticName.end(),
                           semanticName.begin(), ::toupper);
            semanticNames.push_back(semanticName);

            // Handle Mat3/Mat4 types (multiple elements)
            if (element.Type == ShaderDataType::Mat3)
            {
                for (uint32 row = 0; row < 3; ++row)
                {
                    D3D11_INPUT_ELEMENT_DESC desc = {};
                    desc.SemanticName = semanticNames.back().c_str();
                    desc.SemanticIndex = row;
                    desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                    desc.InputSlot = 0;
                    desc.AlignedByteOffset = element.Offset + (row * 12);
                    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                    desc.InstanceDataStepRate = 0;
                    inputElements.push_back(desc);
                }
            }
            else if (element.Type == ShaderDataType::Mat4)
            {
                for (uint32 row = 0; row < 4; ++row)
                {
                    D3D11_INPUT_ELEMENT_DESC desc = {};
                    desc.SemanticName = semanticNames.back().c_str();
                    desc.SemanticIndex = row;
                    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    desc.InputSlot = 0;
                    desc.AlignedByteOffset = element.Offset + (row * 16);
                    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                    desc.InstanceDataStepRate = 0;
                    inputElements.push_back(desc);
                }
            }
            else
            {
                D3D11_INPUT_ELEMENT_DESC desc = {};
                desc.SemanticName = semanticNames.back().c_str();
                desc.SemanticIndex = 0;
                desc.Format = ShaderDataTypeToDXGIFormat(element.Type);
                desc.InputSlot = 0;
                desc.AlignedByteOffset = element.Offset;
                desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                desc.InstanceDataStepRate = 0;
                inputElements.push_back(desc);
            }
        }

        NS_ENGINE_ASSERT(m_VSBlob, "Vertex shader bytecode not available for input layout creation");

        auto* vsBlob = static_cast<ID3DBlob*>(m_VSBlob);
        HRESULT hr = device->CreateInputLayout(
            inputElements.data(),
            static_cast<UINT>(inputElements.size()),
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            &m_InputLayout
        );

        NS_ENGINE_ASSERT(SUCCEEDED(hr), "Failed to create input layout");
        NS_ENGINE_INFO("Input layout created with {} elements for shader '{}'",
                       inputElements.size(), m_Name);
    }

} // namespace NanSu

#endif // NS_PLATFORM_WINDOWS

#pragma once

#include "Renderer/ConstantBuffer.h"

#ifdef NS_PLATFORM_WINDOWS

// Forward declaration to avoid including DX11 headers in header file
struct ID3D11Buffer;

namespace NanSu
{
    /**
     * @brief DirectX 11 implementation of ConstantBuffer
     *
     * Uses D3D11_USAGE_DYNAMIC for efficient per-frame updates.
     */
    class DX11ConstantBuffer : public ConstantBuffer
    {
    public:
        /**
         * @brief Create a constant buffer with the specified size
         * @param size Size in bytes (will be aligned to 16 bytes)
         */
        DX11ConstantBuffer(uint32 size);
        ~DX11ConstantBuffer();

        void SetData(const void* data, uint32 size) override;
        void Bind(uint32 slot) const override;
        void Unbind(uint32 slot) const override;

    private:
        ID3D11Buffer* m_Buffer = nullptr;
        uint32 m_Size = 0;
    };

}

#endif // NS_PLATFORM_WINDOWS

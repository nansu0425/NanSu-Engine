#pragma once

#include "Core/Types.h"

#include <string>

namespace NanSu
{
    // =========================================================================
    // Texture
    // =========================================================================

    /**
     * @brief Abstract interface for all texture types
     *
     * Base class for texture resources. Derived types (Texture2D, Texture3D, etc.)
     * provide specific functionality.
     */
    class Texture
    {
    public:
        virtual ~Texture() = default;

        // Non-copyable
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        /**
         * @brief Get the texture width in pixels
         * @return Width in pixels
         */
        virtual uint32 GetWidth() const = 0;

        /**
         * @brief Get the texture height in pixels
         * @return Height in pixels
         */
        virtual uint32 GetHeight() const = 0;

        /**
         * @brief Bind the texture to a shader slot
         * @param slot The texture slot (0 = t0, 1 = t1, etc.)
         */
        virtual void Bind(uint32 slot = 0) const = 0;

        /**
         * @brief Unbind the texture from a shader slot
         * @param slot The texture slot to unbind
         */
        virtual void Unbind(uint32 slot = 0) const = 0;

    protected:
        Texture() = default;
    };

    // =========================================================================
    // Texture2D
    // =========================================================================

    /**
     * @brief Abstract interface for 2D textures
     *
     * Use Texture2D::Create() factory methods to create platform-specific textures.
     *
     * Example usage:
     * @code
     * // Load from file
     * auto* texture = Texture2D::Create("Assets/Textures/player.png");
     *
     * // Create empty texture and fill with data
     * auto* texture = Texture2D::Create(width, height);
     * texture->SetData(pixelData, dataSize);
     *
     * // Use in rendering
     * texture->Bind(0);  // Bind to slot t0
     * @endcode
     */
    class Texture2D : public Texture
    {
    public:
        virtual ~Texture2D() = default;

        /**
         * @brief Set texture data from raw pixel data
         * @param data Pointer to RGBA pixel data (4 bytes per pixel)
         * @param size Size of data in bytes (must be width * height * 4)
         */
        virtual void SetData(const void* data, uint32 size) = 0;

        /**
         * @brief Create a 2D texture from a file
         * @param filePath Path to the image file (PNG, JPG, BMP, TGA, etc.)
         * @return Pointer to the created texture (caller owns memory)
         *
         * Supported formats: PNG, JPEG, BMP, TGA, GIF, HDR, PSD, PNM
         */
        static Texture2D* Create(const std::string& filePath);

        /**
         * @brief Create an empty 2D texture with specified dimensions
         * @param width Texture width in pixels
         * @param height Texture height in pixels
         * @return Pointer to the created texture (caller owns memory)
         *
         * Use SetData() to populate the texture after creation.
         */
        static Texture2D* Create(uint32 width, uint32 height);

    protected:
        Texture2D() = default;
    };

} // namespace NanSu

#pragma once

#include "Resource/Resource.h"
#include "Vulkan.h"

namespace bl {

/// @brief 
enum class TextureFileType {
    ePNG,
    eQOI,
};

/// @brief Pixel formats that are currently supported for a texture object. 
enum class TextureFormat {
    eRGB,
    eRGBA,
};

/// @brief Texture color spaces that are currently supported.
enum class TextureColorSpace {
    eSRGB,
    eLinear,
};

/// @brief Generic texture loading resource interface.
///
/// This is a generic texture loader and will load the file generically of 
/// type, size, or color space. It's useful for further basic textures to
/// inherit from this or for building other resources. Texture resources are
/// probably only going to be loaded for a short period of time until the
/// file gets uploaded to the GPU and the resource manager will automatically
/// unload this resource.
class Texture : public Resource {
public:

    /// @brief Texture Constructor
    Texture(const nlohmann::json& data);

    /// @brief Destructor
    virtual ~Texture();

    /// @brief Loads the texture file into memory.
    virtual void Load() override;

    /// @brief Unloads the texture file from memory.
    virtual void Unload() override;

    /// @brief GetExtent
    /// @returns The size of the texture in pixels.
    VkExtent2D GetExtent() const;

    TextureFormat GetFormat() const;
    TextureColorSpace GetColorSpace() const;

    std::span<const std::byte> GetImageData() const;

private:
    void DecodePNG(const std::vector<std::byte>& buffer);
    void DecodeQOI(const std::vector<std::byte>& buffer);

    VkExtent2D _extent;
    TextureFileType _type; 
    TextureFormat _format;
    TextureColorSpace _colorSpace;
    std::vector<std::byte> _imageData;
};

} // namespace bl

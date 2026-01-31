#pragma once

#include <assets_format/texture_format.hpp>

// OpenGL texture format constants
// These match the compressed texture formats that OpenGL supports natively

namespace OpenGLFormat
{
    // For use with glCompressedTexImage2D
    constexpr unsigned int GL_COMPRESSED_RGB_S3TC_DXT1_EXT = 0x83F0;
    constexpr unsigned int GL_COMPRESSED_RGBA_S3TC_DXT1_EXT = 0x83F1;
    constexpr unsigned int GL_COMPRESSED_RGBA_S3TC_DXT5_EXT = 0x83F3;

    // Helper function to convert TextureFormat to OpenGL format
    inline unsigned int GetOpenGLFormat(TextureFormat format, bool hasAlpha)
    {
        switch (format)
        {
        case TextureFormat::DXT1:
            return hasAlpha ? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        case TextureFormat::DXT5:
            return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        case TextureFormat::UNCOMPRESSED_RGBA:
        case TextureFormat::UNCOMPRESSED_RGB:
        default:
            return 0; // Use regular glTexImage2D for uncompressed
        }
    }

    // Check if format is compressed
    inline bool IsCompressed(TextureFormat format)
    {
        return format == TextureFormat::DXT1 || format == TextureFormat::DXT5;
    }
}

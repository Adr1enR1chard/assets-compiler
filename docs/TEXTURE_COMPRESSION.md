# Texture Compression System

## Overview
The asset compiler now supports DXT texture compression (BC1/BC3) that can be directly loaded by OpenGL using `glCompressedTexImage2D`.

## Compression Formats

### DXT1 (BC1)
- **Use case**: RGB textures or RGBA with 1-bit alpha
- **Compression**: 8 bytes per 4x4 block
- **Ratio**: 6:1 for RGB, 8:1 for RGBA
- **OpenGL constant**: `GL_COMPRESSED_RGB_S3TC_DXT1_EXT` or `GL_COMPRESSED_RGBA_S3TC_DXT1_EXT`

### DXT5 (BC3)
- **Use case**: RGBA textures with full alpha gradient
- **Compression**: 16 bytes per 4x4 block  
- **Ratio**: 4:1
- **OpenGL constant**: `GL_COMPRESSED_RGBA_S3TC_DXT5_EXT`

## File Format

### TextureHeader Structure
```cpp
struct TextureHeader
{
    uint32_t magic;        // 'TEX0' (0x30584554)
    uint32_t width;        // Texture width
    uint32_t height;       // Texture height
    uint32_t channels;     // Original channel count
    uint32_t mipLevels;    // Mipmap levels (currently 1)
    TextureFormat format;  // Compression format
    uint32_t dataSize;     // Compressed data size in bytes
};
```

### TextureFormat Enum
```cpp
enum class TextureFormat : uint32_t
{
    UNCOMPRESSED_RGBA = 0,
    UNCOMPRESSED_RGB = 1,
    DXT1 = 2,  // BC1
    DXT5 = 3   // BC3
};
```

## Compilation

### Automatic Format Selection
The compiler automatically chooses the compression format:
- **DXT1**: Used for RGB textures (channels < 4)
- **DXT5**: Used for RGBA textures (channels == 4)

### Usage
```bash
assets_compiler input_texture.png output_dir
```

The compiler will:
1. Load the texture
2. Compress it in 4x4 blocks using stb_dxt
3. Write the compressed data with header
4. Output compression statistics

## Loading Compressed Textures

### Using LoadedTexture
```cpp
#include <assets_loader/texture_loader.hpp>
#include <assets_format/opengl_format.hpp>

LoadedTexture texture;
if (TextureLoader::LoadTexture("texture_path", texture))
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    if (OpenGLFormat::IsCompressed(texture.format))
    {
        GLuint glFormat = OpenGLFormat::GetOpenGLFormat(
            texture.format, 
            texture.channels == 4
        );
        
        glCompressedTexImage2D(
            GL_TEXTURE_2D,
            0,
            glFormat,
            texture.width,
            texture.height,
            0,
            texture.data.size(),
            texture.data.data()
        );
    }
}
```

## OpenGL Requirements

### Extension Support
DXT compression requires the OpenGL extension:
- `GL_EXT_texture_compression_s3tc`

Check for support:
```cpp
// With GLEW
if (GLEW_EXT_texture_compression_s3tc)
{
    // DXT compression is supported
}

// Or manually
if (glfwExtensionSupported("GL_EXT_texture_compression_s3tc"))
{
    // DXT compression is supported
}
```

### Desktop vs Mobile
- **Desktop GPUs**: DXT/S3TC widely supported (NVIDIA, AMD, Intel)
- **Mobile GPUs**: May need ETC2 or ASTC instead (not implemented yet)

## Benefits

1. **Reduced Memory Usage**: 4:1 to 8:1 compression ratio
2. **Faster Loading**: Less data to read from disk
3. **Better Performance**: GPU can directly use compressed data
4. **No Runtime Overhead**: Decompression happens in GPU hardware

## Limitations

1. **Block-based**: Textures are processed in 4x4 blocks (automatically padded)
2. **Lossy**: Some quality loss compared to original (minimal with HIGH_QUAL mode)
3. **No Mipmaps**: Currently only supports single mipmap level (can be extended)
4. **Platform-specific**: DXT is best for desktop; mobile may need different formats

## Future Enhancements

Potential additions:
- Mipmap generation
- ETC2/ASTC for mobile platforms
- Runtime format selection based on hardware
- Uncompressed fallback option
- BC4/BC5 for normal maps

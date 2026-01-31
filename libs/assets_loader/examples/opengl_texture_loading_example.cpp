// Example: How to load compressed textures in OpenGL
//
// This example demonstrates how to use the LoadedTexture structure
// with glCompressedTexImage2D to load DXT compressed textures directly into OpenGL

#include <assets_loader/texture_loader.hpp>
#include <assets_format/opengl_format.hpp>
// #include <GL/glew.h> or your OpenGL headers

/*
void LoadTextureToOpenGL(const std::string& texturePath, GLuint& textureID)
{
    LoadedTexture texture;
    if (!TextureLoader::LoadTexture(texturePath, texture))
    {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Check if texture is compressed
    if (OpenGLFormat::IsCompressed(texture.format))
    {
        // Use glCompressedTexImage2D for compressed textures
        GLuint glFormat = OpenGLFormat::GetOpenGLFormat(texture.format, texture.channels == 4);

        glCompressedTexImage2D(
            GL_TEXTURE_2D,           // target
            0,                       // level
            glFormat,                // internal format (DXT1/DXT5)
            texture.width,           // width
            texture.height,          // height
            0,                       // border (must be 0)
            texture.data.size(),     // image size in bytes
            texture.data.data()      // compressed data
        );

        std::cout << "Loaded compressed texture: " << texture.width << "x" << texture.height
                  << " (" << texture.data.size() << " bytes)" << std::endl;
    }
    else
    {
        // Use regular glTexImage2D for uncompressed textures
        GLuint format = (texture.channels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            texture.width,
            texture.height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            texture.data.data()
        );
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Optionally generate mipmaps (not for compressed textures in this version)
    // glGenerateMipmap(GL_TEXTURE_2D);
}
*/

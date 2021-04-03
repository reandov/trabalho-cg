#include <GL/gl.h>
#include "Texture.h"
#include "stb_image.h"

Texture::Texture() :

                     m_id(0), m_width(0),
                     m_height(0), m_channels(0)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

// Function used to load the textures at given filepath
void Texture::load(std::string filePath)
{
    unsigned char *imgData;

    // Flip texture when needed
    stbi_set_flip_vertically_on_load(true);

    // Load the image's data
    imgData = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 4);

    // If the image was successfully loaded
    if (imgData)
    {
        // Generate texture with OpenGL
        glGenTextures(1, &m_id);

        // Bind to a texture id
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Configure texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, imgData);

        // Filters and other configurations
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Frees the space
        stbi_image_free(imgData);
    }
    // Throws in case of missing texture
    else
    {
        std::cout << "ERRO :: Nao foi possivel carregar a textura" << filePath.c_str() << std::endl;
    }
}

// Bind texture
void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

// Unbind texture
void Texture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
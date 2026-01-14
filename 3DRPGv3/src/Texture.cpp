#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

Texture::Texture(const char* filePath, bool flipVertically) : Width(0), Height(0), Channels(0) {
    stbi_set_flip_vertically_on_load(flipVertically);
    
    unsigned char* data = stbi_load(filePath, &Width, &Height, &Channels, 0);
    
    if (!data) {
        std::cout << "Failed to load texture: " << filePath << std::endl;
        // Create a simple white texture as fallback
        Width = Height = 1;
        Channels = 3;
        unsigned char white[] = {255, 255, 255};
        data = white;
    }
    
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format = (Channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    if (data && Width > 1) {
        stbi_image_free(data);
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

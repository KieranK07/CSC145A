#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

class Texture {
public:
    unsigned int ID;
    int Width, Height, Channels;

    Texture(const char* filePath, bool flipVertically = true);
    Texture(const unsigned char* data, int width, int height, int channels); // For embedded textures
    ~Texture();
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
};

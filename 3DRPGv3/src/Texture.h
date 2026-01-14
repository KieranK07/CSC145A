#pragma once
#include <glad/glad.h>
#include <string>

class Texture {
public:
    unsigned int ID;
    int Width, Height, Channels;

    Texture(const char* filePath, bool flipVertically = true);
    ~Texture();
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
};

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 WorldPos;

uniform vec3 objectColor;
uniform sampler2D textureSampler;
uniform bool useTexture;
uniform float tilingFactor = 1.0;

void main() {
    if (useTexture) {
        // Apply tiling factor to the UV coordinates
        vec2 uv = TexCoord * tilingFactor;
        
        // Sample the texture with properly wrapped UVs
        FragColor = texture(textureSampler, uv);
    } else {
        FragColor = vec4(objectColor, 1.0);
    }
}
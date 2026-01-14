#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 objectColor;
uniform sampler2D textureSampler;
uniform bool useTexture;

void main() {
    if (useTexture) {
        FragColor = texture(textureSampler, TexCoord);
    } else {
        FragColor = vec4(objectColor, 1.0);
    }
}
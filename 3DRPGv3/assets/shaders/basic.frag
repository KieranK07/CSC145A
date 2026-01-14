#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 WorldPos;

uniform vec3 objectColor;
uniform sampler2D textureSampler;
uniform bool useTexture;
uniform float tilingFactor = 1.0;

// Pseudo-random function
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    if (useTexture) {
        vec2 tiledTexCoord = TexCoord * tilingFactor;
        
        // Get tile coordinates
        vec2 tilePos = floor(WorldPos.xz);
        float tileRandom = random(tilePos);
        
        // Rotate UV based on tile random value (0, 90, 180, 270 degrees)
        vec2 uv = fract(tiledTexCoord);
        vec2 center = vec2(0.5);
        
        // Determine rotation (4 rotations: 0, 90, 180, 270)
        float rotationChoice = floor(tileRandom * 4.0);
        
        if (rotationChoice == 1.0) {
            // 90 degrees
            uv = vec2(1.0 - uv.y, uv.x);
        } else if (rotationChoice == 2.0) {
            // 180 degrees
            uv = vec2(1.0 - uv.x, 1.0 - uv.y);
        } else if (rotationChoice == 3.0) {
            // 270 degrees
            uv = vec2(uv.y, 1.0 - uv.x);
        }
        
        // Add the tiled integer part back
        uv += floor(tiledTexCoord);
        
        FragColor = texture(textureSampler, uv);
    } else {
        FragColor = vec4(objectColor, 1.0);
    }
}
#version 330 core

in vec2 texCoords;

uniform sampler2D screenTexture;

out vec4 fragColor;

void main() 
{
    float depthValue = texture(screenTexture, texCoords).a;
    if (depthValue > 0.0) {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
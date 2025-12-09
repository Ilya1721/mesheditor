#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 screenTextureCoordsIn;

out vec2 screenTextureCoords;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    screenTextureCoords = screenTextureCoordsIn;
}
#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
    gl_Position = lightProjection * lightView * model * vec4(pos, 1.0);
}
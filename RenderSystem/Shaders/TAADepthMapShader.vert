#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 jitteredProjection;

void main()
{
    gl_Position = jitteredProjection * view * model * vec4(pos, 1.0);
}
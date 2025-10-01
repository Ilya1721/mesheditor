#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;

out vec3 skyboxCoords;

void main()
{
    skyboxCoords = pos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(pos, 1.0); 
    gl_Position = pos.xyww;
}

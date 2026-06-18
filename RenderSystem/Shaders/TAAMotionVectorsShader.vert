#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 prevModel;
uniform mat4 currModel;
uniform mat4 prevView;
uniform mat4 currView;
uniform mat4 projection;

out vec4 prevClipPos;
out vec4 currClipPos;

void main() 
{
    vec4 prevWorldPos = prevModel * vec4(pos, 1.0);
    vec4 currWorldPos = currModel * vec4(pos, 1.0);

    prevClipPos = projection * prevView * prevWorldPos;
    currClipPos = projection * currView * currWorldPos;

    gl_Position = currClipPos;
}
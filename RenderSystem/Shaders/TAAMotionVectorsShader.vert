#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 prevModel;
uniform mat4 currentModel;
uniform mat4 prevView;
uniform mat4 currentView;
uniform mat4 projection;

out vec4 prevClipPos;
out vec4 currentClipPos;

void main() 
{
    vec4 prevWorldPos = prevModel * vec4(pos, 1.0);
    vec4 currentWorldPos = currentModel * vec4(pos, 1.0);

    prevClipPos = projection * prevView * prevWorldPos;
    currentClipPos = projection * currentView * currentWorldPos;

    gl_Position = currentClipPos;
}
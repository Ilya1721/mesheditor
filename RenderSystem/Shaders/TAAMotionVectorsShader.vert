#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 prevModel;
uniform mat4 currentModel;
uniform mat4 prevView;
uniform mat4 currentView;
uniform mat4 prevJitteredProjection;
uniform mat4 currentJitteredProjection;

out vec4 prevClipPos;
out vec4 currentClipPos;

void main() 
{
    vec4 prevWorldPos = prevModel * vec4(pos, 1.0);
    vec4 currentWorldPos = currentModel * vec4(pos, 1.0);

    prevClipPos = prevJitteredProjection * prevView * prevWorldPos;
    currentClipPos = currentJitteredProjection * currentView * currentWorldPos;

    gl_Position = currentClipPos;
}
#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 prevView;
uniform mat4 currentView;
uniform mat4 projection;
uniform mat4 jitteredProjection;

out vec4 currentClipPos;
out vec4 prevClipPos;

void main() 
{
    vec4 worldPos = model * vec4(pos, 1.0);

    currentClipPos = jitteredProjection * currentView * worldPos;
    prevClipPos = projection * prevView * worldPos;

    gl_Position = currentClipPos;
}
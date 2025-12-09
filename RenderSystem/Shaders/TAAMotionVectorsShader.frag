#version 330 core

in vec4 prevClipPos;
in vec4 currentClipPos;

layout(location = 0) out vec2 motionVec;

void main()
{
    vec2 prevNDC = prevClipPos.xy / prevClipPos.w;
    vec2 currentNDC = currentClipPos.xy / currentClipPos.w;

    vec2 prevUV = prevNDC * 0.5 + 0.5;
    vec2 currentUV = currentNDC * 0.5 + 0.5;

    motionVec = currentUV - prevUV;
}
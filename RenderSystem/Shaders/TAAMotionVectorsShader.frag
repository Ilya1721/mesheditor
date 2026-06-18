#version 330 core

in vec4 prevClipPos;
in vec4 currClipPos;

layout(location = 0) out vec2 motionVec;

void main()
{
    vec2 prevNDC = prevClipPos.xy / prevClipPos.w;
    vec2 currNDC = currClipPos.xy / currClipPos.w;

    vec2 prevUV = prevNDC * 0.5 + 0.5;
    vec2 currUV = currNDC * 0.5 + 0.5;

    motionVec = currUV - prevUV;
}
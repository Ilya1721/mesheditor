#version 330 core

in vec2 screenTexture;

out vec4 fragColor;

uniform sampler2D currColorTexture;
uniform sampler2D prevColorTexture;
uniform sampler2D depthMap;
uniform sampler2D motionVectorsTexture;

uniform mat4 prevViewProj;
uniform mat4 invCurrViewProj;

uniform float blendFactor = 0.9;
uniform bool isFirstFrame;

vec3 reconstructWorldPosition(vec2 uv, float depth)
{
    vec4 clipPos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 worldPos = invCurrViewProj * clipPos;
    return worldPos.xyz / worldPos.w;
}

vec2 reprojectUV(vec2 uv, float depth)
{
    vec3 worldPos = reconstructWorldPosition(uv, depth);
    vec4 prevClip = prevViewProj * vec4(worldPos, 1.0);
    prevClip /= prevClip.w;
    return prevClip.xy * 0.5 + 0.5;
}

void main()
{
    /*vec4 currentColor = texture(currColorTexture, screenTexture);

    if (isFirstFrame) {
        fragColor = currentColor;
        return;
    }

    float currentDepth = texture(depthMap, screenTexture).r;
    vec2 prevUV = reprojectUV(screenTexture, currentDepth);
    prevUV += texture(motionVectorsTexture, screenTexture).xy;

    vec4 historyColor = texture(prevColorTexture, prevUV);
    float prevDepth = texture(depthMap, prevUV).r;
    float depthDiff = abs(prevDepth - currentDepth);
    float historyWeight = (depthDiff < 0.001) ? blendFactor : 0.0;

    fragColor = mix(currentColor, historyColor, historyWeight);*/
    fragColor = texture(currColorTexture, screenTexture);
}
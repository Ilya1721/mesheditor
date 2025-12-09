#version 330 core

in vec2 screenTextureCoords;

out vec4 fragColor;

uniform sampler2D currColorTexture;
uniform sampler2D prevColorTexture;
uniform sampler2D depthMap;
uniform sampler2D motionVectorsTexture;

uniform mat4 prevViewProj;
uniform mat4 invCurrViewProj;
uniform bool isFirstFrame;
uniform vec2 screenSize;

float maxWeight = 0.9;
float minWeight = 0.1;

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

bool nearlyEqual(float a, float b)
{
    return abs(a - b) < 1e-5;
}

void main()
{
    float currentDepth = texture(depthMap, screenTextureCoords).r;
    vec2 prevUV = reprojectUV(screenTextureCoords, currentDepth);
    vec2 motionVector = texture(motionVectorsTexture, screenTextureCoords).xy;
    prevUV += motionVector;
    vec2 prevColorTextureCoords = nearlyEqual(currentDepth, 1.0) ? screenTextureCoords : prevUV;

    vec4 historyColor = texture(prevColorTexture, prevColorTextureCoords);
    float prevDepth = texture(depthMap, prevColorTextureCoords).r;
    float depthDiff = abs(prevDepth - currentDepth);
    float speed = length(motionVector);
    float maxSpeed = screenSize.x * 0.1;
    float motionFactor = clamp(1.0 - speed / maxSpeed, 0.0, 1.0);
    float blendFactor = mix(minWeight, maxWeight, motionFactor);
    float historyWeight = (depthDiff < 0.001) ? blendFactor : 0.0;

    vec4 currentColor = texture(currColorTexture, screenTextureCoords);
    vec4 mixedColor = mix(currentColor, historyColor, historyWeight);
    fragColor = isFirstFrame ? currentColor : mixedColor;
}
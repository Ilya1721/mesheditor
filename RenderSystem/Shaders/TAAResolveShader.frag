#version 330 core

in vec2 currentUV;

out vec4 fragColor;

uniform sampler2D currColorTexture;
uniform sampler2D prevColorTexture;
uniform sampler2D prevDepthMap;
uniform sampler2D currDepthMap;
uniform sampler2D motionVectorsTexture;

uniform bool isFirstFrame;
uniform vec2 screenSize;

float maxWeight = 0.95;
float minWeight = 0.05;
float maxSpeedPixels = 64.0;

vec3 getTexelColor(vec2 uv) {
    return texture(currColorTexture, uv).rgb;
}

vec3 clampRGB(vec2 uv, vec3 color) {
    vec2 texel = 1.0 / screenSize;

    vec3 texelColor = getTexelColor(uv);
    vec3 rightTexelColor = getTexelColor(uv + vec2(texel.x, 0));
    vec3 leftTexelColor = getTexelColor(uv - vec2(texel.x, 0));
    vec3 upTexelColor = getTexelColor(uv + vec2(0, texel.y));
    vec3 downTexelColor = getTexelColor(uv - vec2(0, texel.y));

    vec3 minLeftRightColor = min(leftTexelColor, rightTexelColor);
    vec3 maxLeftRightColor = max(leftTexelColor, rightTexelColor);
    vec3 minDownUpColor = min(downTexelColor, upTexelColor);
    vec3 maxDownUpColor = max(downTexelColor, upTexelColor);

    vec3 minColor = min(texelColor, min(minLeftRightColor, minDownUpColor));
    vec3 maxColor = max(texelColor, max(maxLeftRightColor, maxDownUpColor));

    return clamp(color, minColor, maxColor);
}

void main()
{
    vec2 motionUV = texture(motionVectorsTexture, currentUV).xy;
    float speedPixels = length(motionUV * screenSize);
    float motionFactor = clamp(1.0 - speedPixels / maxSpeedPixels, 0.0, 1.0);
    float blendFactor = mix(minWeight, maxWeight, motionFactor);

    vec2 prevUV = currentUV - motionUV;
    float currentDepth = texture(currDepthMap, currentUV).r;
    float prevDepth = texture(prevDepthMap, prevUV).r;
    bool depthNotChanged = abs(prevDepth - currentDepth) < currentDepth * 0.02;
    bool acceptHistory = depthNotChanged && !isFirstFrame;
    float historyWeight = acceptHistory ? blendFactor : 0.0;

    vec4 currentColor = texture(currColorTexture, currentUV);
    vec4 historyColor = texture(prevColorTexture, prevUV);
    historyColor.rgb = clampRGB(currentUV, historyColor.rgb);
    fragColor = mix(currentColor, historyColor, historyWeight);
}
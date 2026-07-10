#version 330 core

in vec4 fragPosLightSpace;

uniform float shadowBias;
uniform sampler2D shadowMap;

out vec4 fragColor;

float getShadowFactor()
{
    vec3 fragPosClipSpace = fragPosLightSpace.xyz / fragPosLightSpace.w;
    vec3 fragPosNDC = fragPosClipSpace * 0.5 + 0.5;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));

    float shadow = 0.0;
    for (int x = -1; x <= 1; ++x)
    {
      for (int y = -1; y <= 1; ++y)
      {
        vec2 textureCoords = fragPosNDC.xy + vec2(x, y) * texelSize;
        float fragDepthOnTexture = texture(shadowMap, textureCoords).r;
        float fragDepthOnScreen = fragPosNDC.z;
        shadow += (fragDepthOnScreen - shadowBias < fragDepthOnTexture) ? 1.0 : 0.5;
      }
    }

    return shadow / 9.0;
}

void main()
{
  fragColor = vec4(vec3(getShadowFactor()), 1.0);
}

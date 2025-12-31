#version 330 core

in vec4 fragPosLightSpace;

uniform float shadowBias;
uniform sampler2D shadowMap;

out vec4 fragColor;

float getShadowFactor()
{
  vec3 fragPosClipSpace = fragPosLightSpace.xyz / fragPosLightSpace.w;
  vec3 fragPosNDC = fragPosClipSpace * 0.5 + 0.5;

  float fragDepthOnTexture = texture(shadowMap, fragPosNDC.xy).r;
  float fragDepthOnScreen = fragPosNDC.z;
  float shadow = (fragDepthOnScreen - shadowBias < fragDepthOnTexture) ? 1.0 : 0.5;

  return shadow;
}

void main()
{
  fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

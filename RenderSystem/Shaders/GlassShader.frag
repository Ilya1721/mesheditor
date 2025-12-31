#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;

uniform vec3 cameraPos;
uniform samplerCube skybox;

struct GlassMaterial
{
  float refractiveIndex;
  float reflectionStrength;
  float transparency;
  float interpolationFactor;
  vec3 color;
};

uniform GlassMaterial material;

out vec4 fragColor;

vec3 computeColor()
{
  vec3 viewDir = normalize(vertexPos - cameraPos);
  vec3 normal = normalize(vertexNormal);
  vec3 reflectionDir = reflect(viewDir, normal);
  vec3 refractionDir = refract(viewDir, normal, 1.0 / material.refractiveIndex);

  float fresnel = pow(1.0 - dot(-viewDir, normal), 3.0);
  fresnel = clamp(fresnel, 0.0, 1.0);
  vec3 reflectedColor = texture(skybox, reflectionDir).rgb;
  vec3 refractedColor = texture(skybox, refractionDir).rgb;

  vec3 color = mix(refractedColor, reflectedColor, fresnel);
  color = mix(color, material.color, material.interpolationFactor);

  return color;
}

void main()
{
  vec3 pixelColor = computeColor();
  float transparency = material.transparency;
  fragColor = vec4(pixelColor, transparency);
}

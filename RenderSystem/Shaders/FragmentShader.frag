#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;
in vec4 fragPosLightSpace;

uniform vec3 dirLightPos;
uniform vec3 cameraPos;
uniform float shadowBias;
uniform sampler2D depthMap;
uniform sampler2D diffuseTexture;

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

struct DirectionalLightParams
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform DirectionalLightParams directionalLightParams;

struct PointLightParams
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

struct PointLight
{
  PointLightParams params;
  vec3 pos;
};

#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int pointLightsCount;

out vec4 fragColor;

float getShadowFactor()
{
  vec3 fragPosClipSpace = fragPosLightSpace.xyz / fragPosLightSpace.w;
  vec3 fragPosNDC = fragPosClipSpace * 0.5 + 0.5;

  float fragDepthOnTexture = texture(depthMap, fragPosNDC.xy).r;
  float fragDepthOnScreen = fragPosNDC.z;
  float shadow = (fragDepthOnScreen - shadowBias < fragDepthOnTexture) ? 1.0 : 0.5;

  return shadow;
}

vec3 getDirectionalLight(vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular)
{
  vec3 reversedLightUnitDir = normalize(dirLightPos - vertexPos);
  float diffuseStrength = max(dot(vertexNormal, reversedLightUnitDir), 0.0);
  vec3 diffuse = diffuseStrength * lightDiffuse * material.diffuse;

  vec3 reversedCameraUnitDir = normalize(cameraPos - vertexPos);
  vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormal);
  float specValue =
    pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), material.shininess);
  vec3 specular = specValue * lightSpecular * material.specular;

  vec3 ambient = lightAmbient * material.ambient;
  vec3 pixelColor = ambient + diffuse + specular;

  return pixelColor;
}

float getPointLightAttenuation(PointLight light)
{
  float distanceToLightSource = length(light.pos - vertexPos);
  float linear = light.params.linear * distanceToLightSource;
  float quadratic = light.params.quadratic * pow(distanceToLightSource, 2);
  return 1.0 / (light.params.constant + linear + quadratic);
}

vec3 getPointLights(vec3 pixelColorAfterDirLight)
{
  vec3 pixelColor = vec3(0.0);
  for (int i = 0; i < pointLightsCount; ++i)
  {
    pixelColor += pixelColorAfterDirLight * getPointLightAttenuation(pointLights[i]);
  }
  return pixelColor;
}

void main()
{
  vec3 pixelColor = getDirectionalLight(directionalLightParams.ambient,
                                        directionalLightParams.diffuse,
                                        directionalLightParams.specular);
  pixelColor += getPointLights(pixelColor);
  fragColor = vec4(pixelColor * getShadowFactor(), 1.0);
}

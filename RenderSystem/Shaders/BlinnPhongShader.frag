#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTexture;

uniform vec3 dirLightPos;
uniform vec3 cameraPos;
uniform sampler2D diffuseTexture;

struct BlinnPhongMaterial 
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  bool hasDiffuseTexture;
};

uniform BlinnPhongMaterial material;

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

vec3 blinnPhongDirectional()
{
  vec3 lightAmbient = directionalLightParams.ambient;
  vec3 lightDiffuse = directionalLightParams.diffuse;
  vec3 lightSpecular = directionalLightParams.specular;

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

  if (material.hasDiffuseTexture)
  {
    pixelColor *= texture(diffuseTexture, vertexTexture).rgb;
  } 

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
  vec3 pixelColor = blinnPhongDirectional();
  pixelColor += getPointLights(pixelColor);
  fragColor = vec4(pixelColor, 1.0);
}

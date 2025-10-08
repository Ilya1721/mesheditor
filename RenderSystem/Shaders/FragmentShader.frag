#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec4 fragPosLightSpace;

uniform vec3 dirLightPos;
uniform vec3 cameraPos;
uniform float shadowBias;
uniform sampler2D depthMap;
uniform sampler2D diffuseTexture;
uniform samplerCube skybox;

struct GlassMaterial
{
  float refractiveIndex;
  float reflectionStrength;
  float transparency;
  float interpolationFactor;
  vec3 color;
};

struct BlinnPhongMaterial 
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  bool hasDiffuseTexture;
};

const int BLINN_PHONG_MATERIAL = 0;
const int GLASS_MATERIAL = 1;

struct Material
{
  BlinnPhongMaterial blinnPhong;
  GlassMaterial glass;
  int type;
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

vec3 computeColorForGlassMaterial()
{
  vec3 viewDir = normalize(vertexPos - cameraPos);
  vec3 normal = normalize(vertexNormal);
  vec3 reflectionDir = reflect(viewDir, normal);
  vec3 refractionDir = refract(viewDir, normal, 1.0 / material.glass.refractiveIndex);

  float fresnel = pow(1.0 - dot(-viewDir, normal), 3.0);
  fresnel = clamp(fresnel, 0.0, 1.0);
  vec3 reflectedColor = texture(skybox, reflectionDir).rgb;
  vec3 refractedColor = texture(skybox, refractionDir).rgb;

  vec3 color = mix(refractedColor, reflectedColor, fresnel);
  color = mix(color, material.glass.color, material.glass.interpolationFactor);

  return color;
}

vec3 blinnPhongDirectional()
{
  vec3 lightAmbient = directionalLightParams.ambient;
  vec3 lightDiffuse = directionalLightParams.diffuse;
  vec3 lightSpecular = directionalLightParams.specular;

  vec3 reversedLightUnitDir = normalize(dirLightPos - vertexPos);
  float diffuseStrength = max(dot(vertexNormal, reversedLightUnitDir), 0.0);
  vec3 diffuse = diffuseStrength * lightDiffuse * material.blinnPhong.diffuse;

  vec3 reversedCameraUnitDir = normalize(cameraPos - vertexPos);
  vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormal);
  float specValue =
    pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), material.blinnPhong.shininess);
  vec3 specular = specValue * lightSpecular * material.blinnPhong.specular;

  vec3 ambient = lightAmbient * material.blinnPhong.ambient;
  vec3 pixelColor = ambient + diffuse + specular;

  if (material.blinnPhong.hasDiffuseTexture)
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
  vec3 pixelColor;
  float transparency;

  switch (material.type)
  {
    case BLINN_PHONG_MATERIAL:
      pixelColor = blinnPhongDirectional();
      transparency = 1.0;
      break;
    case GLASS_MATERIAL:
      pixelColor = computeColorForGlassMaterial();
      transparency = material.glass.transparency;
      break;
    default:
      pixelColor = vec3(1.0, 0.0, 1.0);
      break;
  }

  pixelColor += getPointLights(pixelColor);
  fragColor = vec4(pixelColor * getShadowFactor(), transparency);
}

#version 330 core

in vec3 vertexPosCameraSpace;
in vec3 vertexNormalCameraSpace;
in vec4 fragPosLightSpace;

uniform vec3 dirLightPosCameraSpace;
uniform vec3 cameraPosCameraSpace;
uniform float shadowBias;
uniform sampler2D depthMap;

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
  vec3 position;
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

vec3 getDirectionalLight()
{
  vec3 reversedLightUnitDir = normalize(dirLightPosCameraSpace - vertexPosCameraSpace);
  float diffuseStrength = max(dot(vertexNormalCameraSpace, reversedLightUnitDir), 0.0);
  vec3 diffuse = diffuseStrength * directionalLightParams.diffuse * material.diffuse;

  vec3 reversedCameraUnitDir = normalize(cameraPosCameraSpace - vertexPosCameraSpace);
  vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormalCameraSpace);

  float specValue =
    pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), material.shininess);
  vec3 specular = specValue * directionalLightParams.specular * material.specular;

  vec3 ambient = directionalLightParams.ambient * material.ambient;
  vec3 pixelColor = ambient + diffuse + specular;

  return pixelColor;
}

vec3 getPointLight(
  PointLight light,
  vec3 vertexNormalCameraSpace,
  vec3 vertexPosCameraSpace,
  vec3 cameraPosCameraSpace
)
{
  return vec3(0.0);
}

vec3 getPointLights()
{
  vec3 pixelColor = vec3(0.0);
  for (int i = 0; i < pointLightsCount; ++i)
  {
    pixelColor += getPointLight(
      pointLights[i], vertexNormalCameraSpace, vertexPosCameraSpace,
      cameraPosCameraSpace
    );
  }
  return pixelColor;
}

void main()
{
  vec3 pixelColor = getDirectionalLight();
  pixelColor += getPointLights();
  fragColor = vec4(pixelColor * getShadowFactor(), 1.0);
}

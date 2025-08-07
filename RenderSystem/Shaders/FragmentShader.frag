#version 330 core

in vec3 vertexPosInCameraSpace;
in vec3 vertexNormalInCameraSpace;
in vec4 fragPosLightSpace;

uniform vec3 lightSourcePosInCameraSpace;
uniform vec3 cameraPosInCameraSpace;
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

struct DirectionalLight 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directionalLight;

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
    vec3 reversedLightUnitDir = normalize(lightSourcePosInCameraSpace - vertexPosInCameraSpace);
    float diffuseStrength = max(dot(vertexNormalInCameraSpace, reversedLightUnitDir), 0.0);
    vec3 diffuse = diffuseStrength * directionalLight.diffuse * material.diffuse;

    vec3 reversedCameraUnitDir = normalize(cameraPosInCameraSpace - vertexPosInCameraSpace);
    vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormalInCameraSpace);  

    float specValue = pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specValue * directionalLight.specular * material.specular;

    vec3 ambient = directionalLight.ambient * material.ambient;
    vec3 pixelColor = ambient + diffuse + specular;

    return pixelColor;
}

void main()
{
    vec3 pixelColor = getDirectionalLight();
    fragColor = vec4(pixelColor * getShadowFactor(), 1.0);
}

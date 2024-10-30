#version 330 core

in vec3 vertexPosInCameraSpace;
in vec3 vertexNormalInCameraSpace;

uniform vec3 lightSourcePosInCameraSpace;
uniform vec3 cameraPosInCameraSpace;
uniform sampler2D depthMap;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 fragColor;

bool canSeePixelFromLightSource()
{
    return true;
}

float getShadowFactor()
{
    if (!canSeePixelFromLightSource()) 
    {
        return 0.0;
    }

    return 1.0;
}

void main()
{
    vec3 reversedLightUnitDir = normalize(lightSourcePosInCameraSpace - vertexPosInCameraSpace);
    float diffuseStrength = max(dot(vertexNormalInCameraSpace, reversedLightUnitDir), 0.0);
    vec3 diffuse = diffuseStrength * light.diffuse * material.diffuse;

    vec3 reversedCameraUnitDir = normalize(cameraPosInCameraSpace - vertexPosInCameraSpace);
    vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormalInCameraSpace);  

    float specValue = pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specValue * light.specular * material.specular;

    vec3 ambient = light.ambient * material.ambient;
    vec3 pixelColor = ambient + diffuse + specular;

    fragColor = vec4(pixelColor * getShadowFactor(), 1.0);
}

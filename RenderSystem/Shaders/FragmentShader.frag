#version 330 core

in vec3 vertexPosInCameraSpace;
in vec3 vertexNormalInCameraSpace;

uniform vec3 lightPosInCameraSpace;
uniform vec3 cameraPosInCameraSpace;

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

float calcShadowFactor()
{
    return 1.0f;
}

void main()
{
    vec3 reversedLightUnitDir = normalize(lightPosInCameraSpace - vertexPosInCameraSpace);
    float diffuseStrength = max(dot(vertexNormalInCameraSpace, reversedLightUnitDir), 0.0);
    vec3 diffuse = diffuseStrength * light.diffuse * material.diffuse;

    vec3 reversedCameraUnitDir = normalize(cameraPosInCameraSpace - vertexPosInCameraSpace);
    vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormalInCameraSpace);  

    float specValue = pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specValue * light.specular * material.specular;

    vec3 ambient = light.ambient * material.ambient;
    vec3 result = (ambient + diffuse + specular) * calcShadowFactor();
    fragColor = vec4(result, 1.0);
}

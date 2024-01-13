#version 330 core

in vec3 vertexPosInCameraSpace;
in vec3 vertexNormalInCameraSpace;

uniform vec3 lightPosInCameraSpace;
uniform vec3 cameraPosInCameraSpace;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform int shininess;

out vec4 fragColor;

void main()
{
    vec3 reversedLightUnitDir = normalize(lightPosInCameraSpace - vertexPosInCameraSpace);
    float diffuseStrength = max(dot(vertexNormalInCameraSpace, reversedLightUnitDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 reversedCameraUnitDir = normalize(cameraPosInCameraSpace - vertexPosInCameraSpace);
    vec3 reflectDir = reflect(-reversedLightUnitDir, vertexNormalInCameraSpace);  

    float specValue = pow(max(dot(reversedCameraUnitDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * specValue * lightColor;

    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}

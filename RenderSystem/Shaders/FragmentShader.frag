#version 330 core

in vec3 fsNormal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform int shininess;

out vec4 fragColor;

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseStrength = max(dot(fsNormal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 cameraDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fsNormal);  

    float specValue = pow(max(dot(cameraDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * specValue * lightColor;

    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}

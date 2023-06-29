#version 330 core

in vec3 fsNormal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform float ambientStrength;

out vec4 fragColor;

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseStrength = max(dot(fsNormal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diffuseColor;
    vec3 ambient = ambientStrength * ambientColor;
    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}

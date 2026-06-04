#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;
in vec3 vertexColor;

uniform vec3 lightPos;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(vertexNormal);
    vec3 lightDir = normalize(lightPos - vertexPos);
    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 color = vertexColor * (0.75 + 0.75 * diffuse);
    fragColor = vec4(color, 1.0);
}
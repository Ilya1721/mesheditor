#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float pointScale;
uniform float minPointSize;
uniform float maxPointSize;

out vec3 vertexPos;
out vec3 vertexNormal;
out vec3 vertexColor;

void main()
{
    vec4 worldPos = model * vec4(pos, 1.0);
    vertexPos = worldPos.xyz;
    vertexColor = color;
    vertexNormal = vec3(model * vec4(normal, 0.0));

    vec4 viewPos = view * worldPos;
    float dist = -viewPos.z;
    float pointSize = pointScale / dist;
    pointSize = clamp(pointSize, minPointSize, maxPointSize);

    gl_PointSize = pointSize;
    gl_Position = projection * viewPos;
}
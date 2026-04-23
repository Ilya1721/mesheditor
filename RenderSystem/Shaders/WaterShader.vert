#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

out vec3 vertexPos;
out vec3 vertexNormal;
out vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float vTime;

const int MAX_WAVES = 4;
const float PI = 3.14159;

struct Wave 
{
    float amplitude;
    float waveLength;
    float speed;
    vec2 direction;
};

uniform Wave waves[MAX_WAVES];
uniform int waveCount;

void main()
{
    vec3 tempVertexPos = pos;
    float height = 0.0;
    for (int waveIdx = 0; waveIdx < waveCount; waveIdx++)
    {
        Wave wave = waves[waveIdx];
        float spatialFrequency = 2.0 * PI / wave.waveLength;
        float phase = dot(wave.direction, pos.xz);
        height += wave.amplitude * sin(wave.speed * vTime + spatialFrequency * phase);
    }
    tempVertexPos.y += height;

    vec4 worldPos = model * vec4(tempVertexPos, 1.0);
    vertexPos = worldPos.xyz;
    vertexNormal = mat3(model) * normal;
    vertexUV = textureCoords;
    gl_Position = projection * view * worldPos;
}
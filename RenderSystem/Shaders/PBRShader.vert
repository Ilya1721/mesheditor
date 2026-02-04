#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;
layout (location = 3) in vec4 tangent;

out VS_OUT {
    vec3 fragPos;
    vec2 textureCoords;
    mat3 TBN;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 uvScale;

void main()
{
    vec3 T = normalize(mat3(model) * tangent.xyz);
    vec3 N = normalize(mat3(model) * normal);
    vec3 B = cross(N, T) * tangent.w;

    vsOut.TBN = mat3(T, B, N);
    vsOut.fragPos = vec3(model * vec4(pos, 1.0));
    vsOut.textureCoords = uvScale * textureCoords;

    gl_Position = projection * view * vec4(vsOut.fragPos, 1.0);
}

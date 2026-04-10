#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;
layout (location = 3) in vec4 tangent;
layout (location = 4) in vec4 weights;
layout (location = 5) in vec4 jointIndicesFloat;

out VS_OUT {
    vec3 fragPos;
    vec2 textureCoords;
    mat3 TBN;
    vec3 vertexNormal;
} vsOut;

const int MAX_JOINTS = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 uvScale;
uniform bool useSkinningTransform = false;
uniform mat4 skinningTransforms[MAX_JOINTS];

void main()
{
    mat4 skinningTransform = mat4(1.0);
    if (useSkinningTransform)
    {
        ivec4 jointIndices = ivec4(jointIndicesFloat);
        skinningTransform =
            weights.x * skinningTransforms[jointIndices.x] +
            weights.y * skinningTransforms[jointIndices.y] +
            weights.z * skinningTransforms[jointIndices.z] +
            weights.w * skinningTransforms[jointIndices.w];
    }

    mat3 skinnedTBNTransform = mat3(transpose(inverse(model * skinningTransform)));
    vec3 T = normalize(skinnedTBNTransform * tangent.xyz);
    vec3 N = normalize(skinnedTBNTransform * normal);
    vec3 B = cross(N, T) * tangent.w;

    vec4 skinnedPos = skinningTransform * vec4(pos, 1.0);
    vsOut.TBN = mat3(T, B, N);
    vsOut.fragPos = vec3(model * skinnedPos);
    vsOut.textureCoords = uvScale * textureCoords;
    vsOut.vertexNormal = N;

    gl_Position = projection * view * vec4(vsOut.fragPos, 1.0);
}

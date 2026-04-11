#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 4) in vec4 weights;
layout (location = 5) in vec4 jointIndicesFloat;

const int MAX_JOINTS = 100;

uniform mat4 model;
uniform mat4 lightView;
uniform mat4 lightProjection;
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

    vec4 skinnedPos = skinningTransform * vec4(pos, 1.0);
    gl_Position = lightProjection * lightView * model * skinnedPos;
}
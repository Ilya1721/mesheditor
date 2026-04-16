#version 330 core

layout (location = 0) in vec2 textureQuadPos;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 particlePos;
layout (location = 3) in float particleSideLength;
layout (location = 4) in float particleViewDirRotation;
layout (location = 5) in float particleBlendedFrameIdx;
layout (location = 6) in vec4 particleColorMultiplier;

uniform mat4 view;
uniform mat4 projection;
uniform int flipbookCols;
uniform int flipbookRows;

out vec2 adjustedTextureCoords;
out vec4 colorMultiplier;

vec2 rotate(vec2 point, float angle)
{
    float angleCos = cos(angle);
    float angleSin = sin(angle);

    vec2 rotatedPoint;
    rotatedPoint.x = point.x * angleCos - point.y * angleSin;
    rotatedPoint.y = point.x * angleSin + point.y * angleCos;

    return rotatedPoint;
}

void main()
{
    int frameIndex = int(particleBlendedFrameIdx);
    int cellX = frameIndex % flipbookCols;
    int cellY = frameIndex / flipbookCols;
    vec2 cellSize = vec2(1.0 / flipbookCols, 1.0 / flipbookRows);
    vec2 uv = cellSize * (textureCoords + vec2(cellX, cellY));
    adjustedTextureCoords = uv;
    colorMultiplier = particleColorMultiplier;

    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);
    vec2 rotatedTextureQuadPos = rotate(textureQuadPos, particleViewDirRotation);
    vec3 rightMove = cameraRight * rotatedTextureQuadPos.x * particleSideLength;
    vec3 upMove = cameraUp * rotatedTextureQuadPos.y * particleSideLength;
    vec3 worldPos = particlePos + rightMove + upMove;
    gl_Position = projection * view * vec4(worldPos, 1.0);
}
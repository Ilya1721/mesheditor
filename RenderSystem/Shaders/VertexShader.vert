#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 depthTextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPosInCameraSpace;
out vec3 vertexNormalInCameraSpace;

void main()
{
	vertexNormalInCameraSpace = vec3(view * model * vec4(normal, 0.0));
	vertexPosInCameraSpace = vec3(view * model * vec4(pos, 1.0));
	gl_Position = projection * vec4(vertexPosInCameraSpace, 1.0);
}

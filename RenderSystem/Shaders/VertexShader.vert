#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 viewModel;
uniform mat4 projection;

out vec3 vertexPosInCameraSpace;
out vec3 vertexNormalInCameraSpace;

void main()
{
	vertexNormalInCameraSpace = vec3(viewModel * vec4(normal, 0.0));
	vertexPosInCameraSpace = vec3(viewModel * vec4(pos, 1.0));
	gl_Position = projection * vec4(vertexPosInCameraSpace, 1.0);
}

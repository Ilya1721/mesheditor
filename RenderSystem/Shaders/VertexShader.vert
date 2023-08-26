#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 fsNormal;

void main()
{
	mat4 viewModel = view * model;
	fragPos = vec3(viewModel * vec4(pos, 1.0));
	fsNormal = vec3(viewModel * vec4(normal, 0.0));
	gl_Position = projection * vec4(fragPos, 1.0);
}

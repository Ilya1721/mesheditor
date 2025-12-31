#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPos;
out vec3 vertexNormal;

void main()
{
	vertexNormal = vec3(model * vec4(normal, 0.0));
	vertexPos = vec3(model * vec4(pos, 1.0));
	gl_Position = projection * view * vec4(vertexPos, 1.0);
}

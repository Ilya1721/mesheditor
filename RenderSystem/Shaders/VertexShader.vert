#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fsNormal;
out vec3 fragPos;

void main()
{
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(pos, 1.0);
	fsNormal = normal;
	fragPos = vec3(model * vec4(pos, 1.0));
}

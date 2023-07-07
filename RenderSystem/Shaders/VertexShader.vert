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
	fragPos = vec3(view * model * vec4(pos, 1.0));
	gl_Position = projection * vec4(fragPos, 1.0);
	fsNormal = normal;
}

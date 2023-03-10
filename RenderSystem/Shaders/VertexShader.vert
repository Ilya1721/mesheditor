#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 0) in vec3 normalIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPos;

void main()
{
	fragPos = vec3(model * vec4(pos, 1.0));
	gl_Position = projection * view * vec4(fragPos, 1.0);
	normal = normalIn;
}

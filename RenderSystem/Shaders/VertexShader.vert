#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fs_normal;
out vec3 fs_color;

void main()
{
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(pos, 1.0);
	fs_normal = normal;
	fs_color = color;
}

#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;

out vec3 vertexPos;
out vec3 vertexPosCameraSpace;
out vec3 vertexNormalCameraSpace;
out vec4 fragPosLightSpace;

void main()
{
	vertexNormalCameraSpace = vec3(view * model * vec4(normal, 0.0));
	vertexPos = vec3(model * vec4(pos, 1.0));
	vertexPosCameraSpace = vec3(view * vec4(vertexPos, 1.0));
	fragPosLightSpace = lightProjection * lightView * vec4(vertexPos, 1.0);
	gl_Position = projection * vec4(vertexPosCameraSpace, 1.0);
}

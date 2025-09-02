#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;

out vec3 vertexPos;
out vec3 vertexNormal;
out vec2 vertexTexture;
out vec4 fragPosLightSpace;

void main()
{
	vertexTexture = textureCoords;
	vertexNormal = vec3(model * vec4(normal, 0.0));
	vertexPos = vec3(model * vec4(pos, 1.0));
	fragPosLightSpace = lightProjection * lightView * vec4(vertexPos, 1.0);
	gl_Position = projection * view * vec4(vertexPos, 1.0);
}

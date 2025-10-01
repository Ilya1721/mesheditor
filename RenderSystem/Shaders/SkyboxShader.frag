#version 330 core

in vec3 skyboxCoords;

uniform samplerCube skybox;

out vec4 fragColor;

void main()
{
    fragColor = texture(skybox, skyboxCoords);
}

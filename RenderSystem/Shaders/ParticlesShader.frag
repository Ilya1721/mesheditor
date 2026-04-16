#version 330 core

in vec2 adjustedTextureCoords;
in vec4 colorMultiplier;

out vec4 fragColor;

uniform sampler2D flipbookTexture;

void main()
{
    vec4 textureColor = texture(flipbookTexture, adjustedTextureCoords);
    vec4 finalColor = textureColor * colorMultiplier;

    if (finalColor.a < 0.01)
    {
        discard;
    }

    fragColor = finalColor;
}
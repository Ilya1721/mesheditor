#pragma once

#include "ShaderProgram.h"

namespace RenderSystem
{
    class Texture;

    class TextureToScreenShaderProgram : public ShaderProgram
    {
    public:
        TextureToScreenShaderProgram(
            const std::filesystem::path& vertexShaderPath,
            const std::filesystem::path& fragmentShaderPath
        );

        void setTexture(const Texture& texture);

    private:
        void initUniformLocations();

    private:
        int mTexture;
    };
}


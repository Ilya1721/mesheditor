#pragma once

#include "TextureToScreenShaderProgram.h"
#include "RenderBuffer.h"

namespace RenderSystem
{
    class Texture;
    class Renderer;

    class TextureToScreenController
    {
    public:
        TextureToScreenController(Renderer* renderer);

        void render();
        void setTexture(const Texture& texture);

    private:
        TextureToScreenShaderProgram mShaderProgram;
        RenderBuffer mRenderBuffer;
        Renderer* mRenderer;
    };
}

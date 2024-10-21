#pragma once

#include "Texture.h"

namespace RenderSystem
{
    class ImageTexture : public Texture
    {
    public:
        ImageTexture(int width, int height);

        void setDimensions(int width, int height) override;

    private:
        void init();
    };
}


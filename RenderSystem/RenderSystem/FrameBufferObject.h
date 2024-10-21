#pragma once

#include "Texture.h"
#include "Bindable.h"

namespace RenderSystem
{
    class FrameBufferObject : public Bindable
    {
    public:
        FrameBufferObject();

        void attachTexture(const Texture& texture, const std::function<void()>& textureSetupFunc);

    private:
        void init();
        void bind() override;
        void unbind() override;

    private:
       unsigned int mFBO;
    };
}


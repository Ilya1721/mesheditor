#pragma once

#include <functional>

namespace RenderSystem
{
    class Texture
    {
    public:
        Texture(int width, int height);
        virtual ~Texture();

        virtual void setDimensions(int width, int height) = 0;

        void invokeAction(const std::function<void()>& action) const;
        void bind() const;

        unsigned int getId() const;

    private:
        void init();

    protected:
        unsigned int mTexture;
        int mWidth;
        int mHeight;
    };
}

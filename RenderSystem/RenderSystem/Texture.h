#pragma once

#include <functional>

namespace RenderSystem
{
    enum class TextureType
    {
        DEPTH_MAP,
        IMAGE
    };

    class Texture
    {
    public:
        Texture(int width, int height, TextureType textureType);
        ~Texture();

        void write(const std::function<void()>& renderFunc) const;

    private:
        void init(int width, int height);

    private:
        unsigned int mTextureObject;
        TextureType mTextureType;
    };
}

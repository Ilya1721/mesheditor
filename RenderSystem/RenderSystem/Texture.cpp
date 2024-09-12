#include "Texture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace
{
    using namespace RenderSystem;

    constexpr auto TEXTURE_TARGET = GL_TEXTURE_2D;

    int getInternalFormat(TextureType textureType)
    {
        using enum TextureType;

        switch (textureType)
        {
            case DEPTH_MAP:
                return GL_DEPTH_COMPONENT;
            case IMAGE:
                return GL_RGBA;
        }
    }

    int getDrawBuffer(TextureType textureType)
    {
        using enum TextureType;

        switch (textureType)
        {
        case DEPTH_MAP:
            return GL_NONE;
        case IMAGE:
            return GL_BACK;
        }
    }
}

namespace RenderSystem
{
    Texture::Texture(int width, int height, TextureType textureType) :
        mTextureType(textureType)
    {
        init(width, height);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &mTextureObject);
    }

    void Texture::init(int width, int height)
    {
        glGenTextures(1, &mTextureObject);
        glBindTexture(TEXTURE_TARGET, mTextureObject);

        const auto internalFormat = getInternalFormat(mTextureType);
        glTexImage2D(TEXTURE_TARGET, 0, internalFormat, width, height, 0, internalFormat, GL_FLOAT, 0);

        glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(TEXTURE_TARGET, GL_TEXTURE_BORDER_COLOR, borderColor);
    }

    void Texture::write(const std::function<void()>& renderFunc) const
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TEXTURE_TARGET, mTextureObject, 0);

        const auto drawBuffer = getDrawBuffer(mTextureType);
        glDrawBuffer(drawBuffer);
        glReadBuffer(drawBuffer);

        renderFunc();

        const auto defaultDrawBuffer = getDrawBuffer(TextureType::IMAGE);
        glDrawBuffer(defaultDrawBuffer);
        glReadBuffer(defaultDrawBuffer);
    }
}
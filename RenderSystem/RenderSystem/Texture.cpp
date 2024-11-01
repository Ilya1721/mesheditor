#include "Texture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
    Texture::Texture(int width, int height) :
        mWidth(width),
        mHeight(height)
    {
        init();
    }

    Texture::~Texture()
    {
        if (mTexture != 0) {
            glDeleteTextures(1, &mTexture);
        }
    }

    void Texture::invokeEditAction(const std::function<void()>& action)
    {
        GLint textureToRestore;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureToRestore);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        action();
        glBindTexture(GL_TEXTURE_2D, textureToRestore);
    }

    unsigned int Texture::getId() const
    {
        return mTexture;
    }

    void Texture::init()
    {
        glGenTextures(1, &mTexture);
        invokeEditAction([]() {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        });
    }
}

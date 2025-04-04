#include "ImageTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
    ImageTexture::ImageTexture(int width, int height) :
        Texture(width, height)
    {
        init();
    }

    void ImageTexture::setDimensions(int width, int height)
    {
        invokeAction([&width, &height]() {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        });
    }

    void ImageTexture::init()
    {
        setDimensions(mWidth, mHeight);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

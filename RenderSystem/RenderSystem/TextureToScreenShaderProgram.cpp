#include "TextureToScreenShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "Texture.h"

namespace RenderSystem
{
    TextureToScreenShaderProgram::TextureToScreenShaderProgram(
        const std::filesystem::path& vertexShaderPath,
        const std::filesystem::path& fragmentShaderPath
    ) :
        ShaderProgram(vertexShaderPath, fragmentShaderPath)
    {
        initUniformLocations();
    }

    void TextureToScreenShaderProgram::setTexture(const Texture& texture)
    {
        invokeAction([this, &texture]() {
            texture.bind();
        });
    }

    void TextureToScreenShaderProgram::initUniformLocations()
    {
        mTexture = glGetUniformLocation(mShaderProgram, "screenTexture");
    }
}

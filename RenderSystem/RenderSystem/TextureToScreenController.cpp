#include "TextureToScreenController.h"

#include "Renderer.h"

namespace
{
    using namespace RenderSystem;

    const std::string VERTEX_SHADER_PATH = R"(../RenderSystem/Shaders/TextureToScreenShader.vert)";
    const std::string FRAGMENT_SHADER_PATH = R"(../RenderSystem/Shaders/TextureToScreenShader.frag)";

    RenderData getTextureData()
    {
        RenderData renderData;
        renderData.append(TextureData{ Point2D { -1.0f,  1.0f }, Point2D { 0.0f, 1.0f } });
        renderData.append(TextureData{ Point2D { -1.0f, -1.0f }, Point2D { 0.0f, 0.0f } });
        renderData.append(TextureData{ Point2D { 1.0f, -1.0f }, Point2D { 1.0f, 0.0f } });
        renderData.append(TextureData{ Point2D { -1.0f,  1.0f }, Point2D { 0.0f, 1.0f } });
        renderData.append(TextureData{ Point2D { 1.0f, -1.0f }, Point2D { 1.0f, 0.0f } });
        renderData.append(TextureData{ Point2D { 1.0f,  1.0f }, Point2D { 1.0f, 1.0f } });

        return renderData;
    }
}

namespace RenderSystem
{
    TextureToScreenController::TextureToScreenController(Renderer* renderer) :
        mShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
        mRenderer(renderer)
    {}

    void TextureToScreenController::render()
    {
        mShaderProgram.invokeAction([this]() {
            mRenderer->cleanScreen();
            mRenderBuffer.invokeAction([this]() {
                mRenderer->renderRect();
            });
        });
    }

    void TextureToScreenController::setTexture(const Texture& texture)
    {
        mShaderProgram.setTexture(texture);
        mRenderBuffer.invokeAction([this]() {
            mRenderBuffer.loadRenderData(getTextureData(), 2);
        });
    }
}

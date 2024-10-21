#include "ShadowController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/type_ptr.hpp>

namespace RenderSystem
{
    ShadowController::ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath, int windowWidth, int windowHeight) :
        mWindowWidth(windowWidth),
        mWindowHeight(windowHeight),
        mShaderProgram(vertexShaderPath, fragmentShaderPath),
        mTexture(windowWidth, windowHeight),
        mFBO()
    {
        init();
    }

    void ShadowController::calcLightSourceSpace(const Point3D& lightSourcePos)
    {
        const auto lightSourceSpace = glm::lookAt(lightSourcePos, Point3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
        mShaderProgram.invokeAction([this, &lightSourceSpace]() {
            mShaderProgram.setLightSpace(lightSourceSpace);
        });
    }

    void ShadowController::setTextureDimensions(int width, int height)
    {
        mTexture.setDimensions(width, height);
    }

    void ShadowController::renderSceneToDepthMap(const std::function<void()>& renderSceneFunc)
    {
        mFBO.invokeAction([this, &renderSceneFunc]() {
            glViewport(0, 0, mWindowWidth, mWindowHeight);
            glClear(GL_DEPTH_BUFFER_BIT);
            mShaderProgram.invokeAction(renderSceneFunc);
        });
    }

    void ShadowController::init()
    {
        mFBO.attachTexture(mTexture, []() {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        });
    }
}

#include "ShadowController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/type_ptr.hpp>

#include "MeshCore/AABBox.h"

using namespace MeshCore;

namespace
{
    glm::mat4 calcLightSpaceProjectionMatrix(const AABBox& sceneBBox) {
        const auto bboxHeight = sceneBBox.getHeight();
        float orthoSize = bboxHeight / 2.0f;
        float left = -orthoSize;
        float right = orthoSize;
        float bottom = -orthoSize;
        float top = orthoSize;
        float near_plane = 0.1f;
        float far_plane = bboxHeight * 2;

        return glm::ortho(left, right, bottom, top, near_plane, far_plane);
    }
}

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

    void ShadowController::setModel(const glm::mat4& model)
    {
        mShaderProgram.setModel(model);
    }

    void ShadowController::setLightView(const glm::mat4& lightView)
    {
        mShaderProgram.invokeAction([this, &lightView]() {
            mShaderProgram.setLightView(lightView);
        });
    }

    void ShadowController::setLightProjection(const glm::mat4& lightProjection)
    {
        mShaderProgram.invokeAction([this, &lightProjection]() {
            mShaderProgram.setLightProjection(lightProjection);
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

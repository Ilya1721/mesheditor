#pragma once

#include <functional>

#include "GeometryCore/Typedefs.h"

#include "BaseSceneShaderProgram.h"
#include "DepthTexture.h"
#include "FrameBufferObject.h"
#include "Modelable.h"

using namespace GeometryCore;

namespace RenderSystem
{
    class ShadowController : public Modelable
    {
    public:
        ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath, int windowWidth, int windowHeight);

        void setModel(const glm::mat4& model) override;
        void setLightView(const glm::mat4& lightView);
        void setLightProjection(const glm::mat4& lightProjection);
        void setTextureDimensions(int width, int height);
        void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);

    private:
        void init();

    private:
        int mWindowWidth;
        int mWindowHeight;
        BaseSceneShaderProgram mShaderProgram;
        DepthTexture mTexture;
        FrameBufferObject mFBO;
    };
}

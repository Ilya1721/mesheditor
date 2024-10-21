#pragma once

#include <functional>

#include "GeometryCore/Typedefs.h"

#include "DepthMapShaderProgram.h"
#include "DepthTexture.h"
#include "FrameBufferObject.h"

using namespace GeometryCore;

namespace RenderSystem
{
    class ShadowController
    {
    public:
        ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath, int windowWidth, int windowHeight);

        void calcLightSourceSpace(const Point3D& lightSourcePos);
        void setTextureDimensions(int width, int height);

        void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);

    private:
        void init();

    private:
        int mWindowWidth;
        int mWindowHeight;
        DepthMapShaderProgram mShaderProgram;
        DepthTexture mTexture;
        FrameBufferObject mFBO;
    };
}

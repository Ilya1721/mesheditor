#pragma once

#include <functional>

#include "GeometryCore/Typedefs.h"

#include "DepthMapShaderProgram.h"
#include "DepthTexture.h"
#include "FrameBufferObject.h"
#include "Modelable.h"

using namespace GeometryCore;

namespace MeshCore
{
    class AABBox;
}

namespace RenderSystem
{
    class ShadowController : public Modelable
    {
    public:
        ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath, int windowWidth, int windowHeight);

        void setModel(const glm::mat4& model) override;
        void calcLightView(const Point3D& lightSourcePos);
        void calcLightProjection(const MeshCore::AABBox& sceneBBox);
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

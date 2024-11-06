#pragma once

#include "GeometryCore/Typedefs.h"

#include "BaseSceneShaderProgram.h"
#include "Lighting.h"
#include "Modelable.h"

using namespace GeometryCore;

namespace RenderSystem
{
    class SceneShaderProgram : public BaseSceneShaderProgram
    {
        friend class LightSource;

    public:
        SceneShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

        void setCameraPos(const Point3D& cameraPos);
        void setMaterial(const Material& material);
        void setLight(const Light& light);
        void setView(const glm::mat4& view);
        void setProjection(const glm::mat4& projection);
        void setDepthMap(int textureId);

    private:
        void setLightSourcePos(const Point3D& lightSourcePos);
        void initUniformLocations();
        void setUp();

    private:
        Lighting mLighting;
        int mView;
        int mProjection;
        int mShadowBias;
        int mDepthMap;
    };
}


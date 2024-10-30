#pragma once

#include "GeometryCore/Typedefs.h"

#include "ShaderProgram.h"
#include "Lighting.h"
#include "Modelable.h"

using namespace GeometryCore;

namespace RenderSystem
{
    class SceneShaderProgram : public ShaderProgram, public Modelable
    {
        friend class LightSource;

    public:
        SceneShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

        void setCameraPos(const Point3D& cameraPos);
        void setMaterial(const Material& material);
        void setLight(const Light& light);
        void setModel(const glm::mat4& model) override;
        void setView(const glm::mat4& view);
        void setProjection(const glm::mat4& projection);

    private:
        void setLightSourcePos(const Point3D& lightSourcePos);
        void initUniformLocations();

    private:
        Lighting mLighting;
        int mModel;
        int mView;
        int mProjection;
    };
}


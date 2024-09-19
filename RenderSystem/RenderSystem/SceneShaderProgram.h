#pragma once

#include "ShaderProgram.h"
#include "SceneLighting.h"

namespace RenderSystem
{
    class SceneShaderProgram : public ShaderProgram
    {
    public:
        SceneShaderProgram();

        void setView(const float* view) const;
        void setProjection(const float* projection) const;
        void init(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:
        void initUniformLocations();

    public:
        SceneLighting lighting;

    private:
        int mView;
        int mProjection;
    };
}

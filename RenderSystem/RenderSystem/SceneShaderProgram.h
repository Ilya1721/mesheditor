#pragma once

#include "ShaderProgram.h"
#include "SceneLighting.h"

namespace RenderSystem
{
    class SceneShaderProgram : public ShaderProgram
    {
    public:
        SceneShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

        void setView(const float* view) const;
        void setProjection(const float* projection) const;

    private:
        void init();
        void initUniformLocations();

    public:
        SceneLighting lighting;

    private:
        int mView;
        int mProjection;
    };
}

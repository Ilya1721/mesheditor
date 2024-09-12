#pragma once

#include "ShaderProgram.h"

namespace RenderSystem
{
    class DepthShaderProgram : public ShaderProgram
    {
    public:
        DepthShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

        void setLightSpaceTransform(const float* transform) const;

    private:
        void init();
        void initUniformLocations();

    private:
        int mLightSpaceTransform;
    };
}
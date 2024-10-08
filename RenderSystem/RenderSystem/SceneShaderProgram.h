#pragma once

#include "ShaderProgram.h"
#include "Lighting.h"

namespace RenderSystem
{
    class SceneShaderProgram : public ShaderProgram
    {
    public:
        SceneShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

        void setLightPos(const float* lightPos);
        void setCameraPos(const float* cameraPos);
        void setMaterial(const Material& material);
        void setLight(const Light& light);

    private:
        Lighting mLighting;
    };
}


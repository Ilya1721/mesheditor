#include "SceneShaderProgram.h"

namespace RenderSystem
{
    SceneShaderProgram::SceneShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) :
        ShaderProgram(vertexShaderPath, fragmentShaderPath)
    {
        mLighting.init(mShaderProgram);
    }

    void SceneShaderProgram::setLightPos(const float* lightPos)
    {
        mLighting.setLightPos(lightPos);
    }

    void SceneShaderProgram::setCameraPos(const float* cameraPos)
    {
        mLighting.setCameraPos(cameraPos);
    }

    void SceneShaderProgram::setMaterial(const Material& material)
    {
        mLighting.setMaterial(material);
    }

    void SceneShaderProgram::setLight(const Light& light)
    {
        mLighting.setLight(light);
    }
}

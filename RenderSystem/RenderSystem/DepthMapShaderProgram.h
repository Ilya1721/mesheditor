#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
    class DepthMapShaderProgram : public ShaderProgram
    {
    public:
        DepthMapShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);

        void setModel(const glm::mat4& model);
        void setLightSpace(const glm::mat4& lightSpace);

    private:
        void initUniformLocations();

    private:
        int mModel;
        int mLightSpaceMat;
    };
}
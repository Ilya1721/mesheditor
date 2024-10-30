#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Modelable.h"

using namespace std::filesystem;

namespace RenderSystem
{
    class DepthMapShaderProgram : public ShaderProgram, public Modelable
    {
    public:
        DepthMapShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);

        void setModel(const glm::mat4& model) override;
        void setLightView(const glm::mat4& lightView);
        void setLightProjection(const glm::mat4& lightProjection);

    private:
        void initUniformLocations();

    private:
        int mModel;
        int mLightView;
        int mLightProjection;
    };
}
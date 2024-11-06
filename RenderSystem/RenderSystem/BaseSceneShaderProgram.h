#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Modelable.h"

using namespace std::filesystem;

namespace RenderSystem
{
    class BaseSceneShaderProgram : public ShaderProgram, public Modelable
    {
    public:
        BaseSceneShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);
        virtual ~BaseSceneShaderProgram() = default;

        void setModel(const glm::mat4& model) override;
        void setLightView(const glm::mat4& lightView);
        void setLightProjection(const glm::mat4& lightProjection);

    private:
        void initUniformLocations();

    protected:
        int mModel;
        int mLightView;
        int mLightProjection;
    };
}

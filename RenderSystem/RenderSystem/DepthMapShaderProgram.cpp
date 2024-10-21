#include "DepthMapShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
    DepthMapShaderProgram::DepthMapShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath) :
        ShaderProgram(vertexShaderPath, fragmentShaderPath),
        mModel(),
        mLightSpaceMat()
    {
        initUniformLocations();
    }

    void DepthMapShaderProgram::setModel(const glm::mat4& model)
    {
        invokeAction([this, &model]() {
            glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
            });
    }

    void DepthMapShaderProgram::setLightSpace(const glm::mat4& lightSpace)
    {
        invokeAction([this, &lightSpace]() {
            glUniformMatrix4fv(mLightSpaceMat, 1, false, glm::value_ptr(lightSpace));
        });
    }

    void DepthMapShaderProgram::initUniformLocations()
    {
        mModel = glGetUniformLocation(mShaderProgram, "model");
        mLightSpaceMat = glGetUniformLocation(mShaderProgram, "lightSpaceMatrix");
    }
}

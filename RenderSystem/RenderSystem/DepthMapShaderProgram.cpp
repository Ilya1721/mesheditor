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
        mLightView(),
        mLightProjection()
    {
        initUniformLocations();
    }

    void DepthMapShaderProgram::setModel(const glm::mat4& model)
    {
        invokeAction([this, &model]() {
            glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
            });
    }

    void DepthMapShaderProgram::setLightView(const glm::mat4& lightView)
    {
        invokeAction([this, &lightView]() {
            glUniformMatrix4fv(mLightView, 1, false, glm::value_ptr(lightView));
        });
    }

    void DepthMapShaderProgram::setLightProjection(const glm::mat4& lightProjection)
    {
        invokeAction([this, &lightProjection]() {
            glUniformMatrix4fv(mLightProjection, 1, false, glm::value_ptr(lightProjection));
        });
    }

    void DepthMapShaderProgram::initUniformLocations()
    {
        mModel = glGetUniformLocation(mShaderProgram, "model");
        mLightView = glGetUniformLocation(mShaderProgram, "lightView");
        mLightProjection = glGetUniformLocation(mShaderProgram, "lightProjection");
    }
}

#include "SceneShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
    SceneShaderProgram::SceneShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) :
        ShaderProgram(vertexShaderPath, fragmentShaderPath),
        mModel(),
        mView(),
        mProjection()
    {
        mLighting.init(mShaderProgram);
        initUniformLocations();
    }

    void SceneShaderProgram::setLightSourcePos(const Point3D& lightSourcePos)
    {
        invokeAction([this, &lightSourcePos]() {
            mLighting.setLightSourcePos(glm::value_ptr(lightSourcePos));
        });
    }

    void SceneShaderProgram::setCameraPos(const Point3D& cameraPos)
    {
        invokeAction([this, &cameraPos]() {
            mLighting.setCameraPos(glm::value_ptr(cameraPos));
        });
    }

    void SceneShaderProgram::setMaterial(const Material& material)
    {
        invokeAction([this, &material]() {
            mLighting.setMaterial(material);
        });
    }

    void SceneShaderProgram::setLight(const Light& light)
    {
        invokeAction([this, &light]() {
            mLighting.setLight(light);
        });
    }

    void SceneShaderProgram::setModel(const glm::mat4& model)
    {
        invokeAction([this, &model]() {
            glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
        });
    }

    void SceneShaderProgram::setView(const glm::mat4& view)
    {
        invokeAction([this, &view]() {
            glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
        });
    }

    void SceneShaderProgram::setProjection(const glm::mat4& projection)
    {
        invokeAction([this, &projection]() {
            glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
        });
    }

    void SceneShaderProgram::initUniformLocations()
    {
        mModel = glGetUniformLocation(mShaderProgram, "model");
        mView = glGetUniformLocation(mShaderProgram, "view");
        mProjection = glGetUniformLocation(mShaderProgram, "projection");
    }
}

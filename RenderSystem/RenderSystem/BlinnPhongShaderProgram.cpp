#include "BlinnPhongShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Camera.h"
#include "glad/glad.h"

namespace RenderSystem
{
  BlinnPhongShaderProgram::BlinnPhongShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mView(),
      mProjection()
  {
    mDirectionalLight.init(mShaderProgram);
    mPointLights.init(mShaderProgram);
    initUniformLocations();
  }

  void BlinnPhongShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setCameraPos(camera->getEye());
    setView(camera->getViewMatrix());
  }

  void BlinnPhongShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void BlinnPhongShaderProgram::setDirLightSourcePos(const Point3D& lightSourcePos)
  {
    invoke([this, &lightSourcePos]()
           { mDirectionalLight.setLightSourcePos(glm::value_ptr(lightSourcePos)); });
  }

  void BlinnPhongShaderProgram::setCameraPos(const Point3D& cameraPos)
  {
    invoke([this, &cameraPos]()
           { mDirectionalLight.setCameraPos(glm::value_ptr(cameraPos)); });
  }

  void BlinnPhongShaderProgram::setDirLightParams(
    const DirectionalLightParams& params
  )
  {
    invoke([this, &params]() { mDirectionalLight.setParams(params); });
  }

  void BlinnPhongShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void BlinnPhongShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void BlinnPhongShaderProgram::setAmbient(const RGB& ambient)
  {
    invoke([this, &ambient]() { glUniform3fv(mAmbient, 1, glm::value_ptr(ambient)); });
  }

  void BlinnPhongShaderProgram::setDiffuse(const RGB& diffuse)
  {
    invoke([this, &diffuse]() { glUniform3fv(mDiffuse, 1, glm::value_ptr(diffuse)); });
  }

  void BlinnPhongShaderProgram::setSpecular(const RGB& specular)
  {
    invoke([this, &specular]() { glUniform3fv(mSpecular, 1, glm::value_ptr(specular)); });
  }

  void BlinnPhongShaderProgram::setShininess(float shininess)
  {
    invoke([this, &shininess]() { glUniform1f(mShininess, shininess); });
  }

  void BlinnPhongShaderProgram::setDiffuseTexture(ImageTexture* texturePtr)
  {
    invoke(
      [this, texturePtr]()
      {
        if (!texturePtr) { glUniform1i(mHasDiffuseTexture, false); }
        else
        {
          glUniform1i(mHasDiffuseTexture, true);
          texturePtr->passToFragmentShader(mDiffuseTexture, 0);
        }
      }
    );
  }

  PointLight* BlinnPhongShaderProgram::addPointLight(
    const PointLightParams& params, const Point3D& lightSourcePos
  )
  {
    return mPointLights.addLight(params, lightSourcePos);
  }

  void BlinnPhongShaderProgram::removePointLight(unsigned int index)
  {
    mPointLights.removeLight(index);
  }

  void BlinnPhongShaderProgram::setUVScale(const glm::vec2& scale)
  {
    invoke([this, &scale]() { glUniform2fv(mUVScale, 1, glm::value_ptr(scale)); });
  }

  void BlinnPhongShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mUVScale = getUniformLocation("uvScale");
    mAmbient = getUniformLocation("material.ambient");
    mDiffuse = getUniformLocation("material.diffuse");
    mSpecular = getUniformLocation("material.specular");
    mShininess = getUniformLocation("material.shininess");
    mDiffuseTexture = getUniformLocation("diffuseTexture");
    mHasDiffuseTexture = getUniformLocation("material.hasDiffuseTexture");
  }
}  // namespace RenderSystem

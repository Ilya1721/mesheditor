#include "PBRShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Camera.h"
#include "glad/glad.h"

namespace RenderSystem
{
  PBRShaderProgram::PBRShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void PBRShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setView(camera->getViewMatrix());
    setCameraPos(camera->getEye());
  }

  void PBRShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void PBRShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void PBRShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void PBRShaderProgram::setUVScale(const glm::vec2& scale)
  {
    invoke([this, &scale]() { glUniform2fv(mUVScale, 1, glm::value_ptr(scale)); });
  }

  void PBRShaderProgram::setCameraPos(const glm::vec3& cameraPos)
  {
    invoke([this, &cameraPos]()
           { glUniform3fv(mCameraPos, 1, glm::value_ptr(cameraPos)); });
  }

  void PBRShaderProgram::setLightPos(const glm::vec3& lightPos)
  {
    invoke([this, &lightPos]() { glUniform3fv(mLightPos, 1, glm::value_ptr(lightPos)); });
  }

  void PBRShaderProgram::setLightColor(const RGB& lightColor)
  {
    invoke([this, &lightColor]()
           { glUniform3fv(mLightColor, 1, glm::value_ptr(lightColor)); });
  }

  void PBRShaderProgram::setBaseColorTexture(const ImageTexture& texture)
  {
    invoke(
      [this, &texture]()
      {
        texture.passToFragmentShader(mBaseColorTexture, 0);
        glUniform1i(mHasBaseColorTexture, 1);
      }
    );
  }

  void PBRShaderProgram::setNormalTexture(const ImageTexture& texture)
  {
    invoke(
      [this, &texture]()
      {
        texture.passToFragmentShader(mNormalTexture, 1);
        glUniform1i(mHasNormalTexture, 1);
      }
    );
  }

  void PBRShaderProgram::setMetallicRoughnessTexture(const ImageTexture& texture)
  {
    invoke(
      [this, &texture]()
      {
        texture.passToFragmentShader(mMetallicRoughnessTexture, 2);
        glUniform1i(mHasMetallicRoughnessTexture, 1);
      }
    );
  }

  void PBRShaderProgram::setMetallic(float metallic)
  {
    invoke([this, metallic]() { glUniform1f(mMetallic, metallic); });
  }

  void PBRShaderProgram::setRougness(float roughness)
  {
    invoke([this, roughness]() { glUniform1f(mRoughness, roughness); });
  }

  void PBRShaderProgram::setUseSkinningTransform(bool useSkinningTransform)
  {
    invoke([this, useSkinningTransform]()
           { glUniform1i(mUseSkinningTransform, useSkinningTransform); });
  }

  void PBRShaderProgram::setSkinningTransforms(
    const std::vector<glm::mat4>& skinningTransforms
  )
  {
    if (skinningTransforms.empty())
    {
      return;
    }

    invoke(
      [this, &skinningTransforms]()
      {
        glUniformMatrix4fv(
          mSkinningTransforms,
          skinningTransforms.size(),
          GL_FALSE,
          glm::value_ptr(skinningTransforms[0])
        );
      }
    );
  }

  void PBRShaderProgram::setBaseColor(const RGB& color)
  {
    invoke([this, &color]() { glUniform3fv(mBaseColor, 1, glm::value_ptr(color)); });
  }

  void PBRShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mUVScale = getUniformLocation("uvScale");
    mCameraPos = getUniformLocation("cameraPos");
    mLightPos = getUniformLocation("lightPos");
    mLightColor = getUniformLocation("lightColor");
    mBaseColorTexture = getUniformLocation("baseColorTexture");
    mBaseColor = getUniformLocation("baseColor");
    mHasBaseColorTexture = getUniformLocation("hasBaseColorTexture");
    mNormalTexture = getUniformLocation("normalTexture");
    mHasNormalTexture = getUniformLocation("hasNormalTexture");
    mMetallicRoughnessTexture = getUniformLocation("metallicRoughnessTexture");
    mHasMetallicRoughnessTexture = getUniformLocation("hasMetallicRoughnessTexture");
    mUseSkinningTransform = getUniformLocation("useSkinningTransform");
    mSkinningTransforms = getUniformLocation("skinningTransforms");
  }
}  // namespace RenderSystem

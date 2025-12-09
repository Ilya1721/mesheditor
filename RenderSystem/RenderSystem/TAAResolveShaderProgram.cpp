#include "TAAResolveShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace RenderSystem
{
  TAAResolveShaderProgram::TAAResolveShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void TAAResolveShaderProgram::setCurrentColorTexture(const TAAColorTexture& texture
  ) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mCurrentColorTexture, 0); });
  }

  void TAAResolveShaderProgram::setPreviousColorTexture(const TAAColorTexture& texture
  ) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mPreviousColorTexture, 1); }
    );
  }

  void TAAResolveShaderProgram::setDepthMap(const DepthTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mDepthMap, 2); });
  }

  void TAAResolveShaderProgram::setMotionVectorsTexture(
    const TAAMotionVectorsTexture& texture
  ) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mMotionVectorsTexture, 3); }
    );
  }

  void TAAResolveShaderProgram::setPrevViewProj(const glm::mat4& prevViewProj)
  {
    invoke([this, &prevViewProj]()
           { glUniformMatrix4fv(mPrevViewProj, 1, false, glm::value_ptr(prevViewProj)); }
    );
  }

  void TAAResolveShaderProgram::setInvCurrViewProj(const glm::mat4& invCurrViewProj)
  {
    invoke(
      [this, &invCurrViewProj]()
      { glUniformMatrix4fv(mInvCurrViewProj, 1, false, glm::value_ptr(invCurrViewProj)); }
    );
  }

  void TAAResolveShaderProgram::setIsFirstFrame(bool isFirstFrame)
  {
    invoke([this, isFirstFrame]() { glUniform1i(mIsFirstFrame, isFirstFrame); });
  }

  void TAAResolveShaderProgram::setScreenSize(const glm::vec2& size)
  {
    
    invoke([this, &size]() { glUniform2fv(mScreenSize, 1, glm::value_ptr(size)); });
  }

  void TAAResolveShaderProgram::initUniformLocations()
  {
    mCurrentColorTexture = getUniformLocation("currColorTexture");
    mPreviousColorTexture = getUniformLocation("prevColorTexture");
    mDepthMap = getUniformLocation("depthMap");
    mMotionVectorsTexture = getUniformLocation("motionVectorsTexture");
    mPrevViewProj = getUniformLocation("prevViewProj");
    mInvCurrViewProj = getUniformLocation("invCurrViewProj");
    mIsFirstFrame = getUniformLocation("isFirstFrame");
    mScreenSize = getUniformLocation("screenSize");
  }
}  // namespace RenderSystem

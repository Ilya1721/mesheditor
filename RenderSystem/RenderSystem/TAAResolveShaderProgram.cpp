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

  void TAAResolveShaderProgram::setPrevDepthMap(const DepthTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mPrevDepthMap, 2); });
  }

  void TAAResolveShaderProgram::setCurrDepthMap(const DepthTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mCurrDepthMap, 3); });
  }

  void TAAResolveShaderProgram::setMotionVectorsTexture(
    const TAAMotionVectorsTexture& texture
  ) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mMotionVectorsTexture, 4); }
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
    mPrevDepthMap = getUniformLocation("prevDepthMap");
    mCurrDepthMap = getUniformLocation("currDepthMap");
    mMotionVectorsTexture = getUniformLocation("motionVectorsTexture");
    mIsFirstFrame = getUniformLocation("isFirstFrame");
    mScreenSize = getUniformLocation("screenSize");
  }
}  // namespace RenderSystem

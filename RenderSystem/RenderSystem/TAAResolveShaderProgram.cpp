#include "TAAResolveShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace
{
  constexpr int PREV_DEPTH_MAP_TEXTURE_UNIT = 0;
  constexpr int CURR_DEPTH_MAP_TEXTURE_UNIT = 1;
  constexpr int PREV_COLOR_TEXTURE_UNIT = 2;
  constexpr int CURR_COLOR_TEXTURE_UNIT = 3;
  constexpr int MOTION_VECTORS_TEXTURE_UNIT = 4;
}

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

  void TAAResolveShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(PREV_DEPTH_MAP_TEXTURE_UNIT, mPrevDepthMapId);
    glBindTextureUnit(CURR_DEPTH_MAP_TEXTURE_UNIT, mCurrDepthMapId);
    glBindTextureUnit(PREV_COLOR_TEXTURE_UNIT, mPrevColorTextureId);
    glBindTextureUnit(CURR_COLOR_TEXTURE_UNIT, mCurrColorTextureId);
    glBindTextureUnit(MOTION_VECTORS_TEXTURE_UNIT, mMotionVectorsTextureId);
  }

  void TAAResolveShaderProgram::setPrevDepthMap(const Texture2D& texture) const
  {
    bind();
    mPrevDepthMapId = texture.getId();
    glUniform1i(mPrevDepthMapLocation, PREV_DEPTH_MAP_TEXTURE_UNIT);
  }

  void TAAResolveShaderProgram::setCurrDepthMap(const Texture2D& texture) const
  {
    bind();
    mCurrDepthMapId = texture.getId();
    glUniform1i(mCurrDepthMapLocation, CURR_DEPTH_MAP_TEXTURE_UNIT);
  }

  void TAAResolveShaderProgram::setPrevColorTexture(const Texture2D& texture) const
  {
    bind();
    mPrevColorTextureId = texture.getId();
    glUniform1i(mPrevColorTextureLocation, PREV_COLOR_TEXTURE_UNIT);
  }

  void TAAResolveShaderProgram::setCurrColorTexture(const Texture2D& texture) const
  {
    bind();
    mCurrColorTextureId = texture.getId();
    glUniform1i(mCurrColorTextureLocation, CURR_COLOR_TEXTURE_UNIT);
  }

  void TAAResolveShaderProgram::setMotionVectorsTexture(const Texture2D& texture) const
  {
    bind();
    mMotionVectorsTextureId = texture.getId();
    glUniform1i(mMotionVectorsTextureLocation, MOTION_VECTORS_TEXTURE_UNIT);
  }

  void TAAResolveShaderProgram::setIsFirstFrame(bool isFirstFrame) const
  {
    bind();
    glUniform1i(mIsFirstFrame, isFirstFrame);
  }

  void TAAResolveShaderProgram::setScreenSize(const glm::vec2& size) const
  {
    bind();
    glUniform2fv(mScreenSize, 1, glm::value_ptr(size));
  }

  void TAAResolveShaderProgram::initUniformLocations()
  {
    mCurrColorTextureLocation = getUniformLocation("currColorTexture");
    mPrevColorTextureLocation = getUniformLocation("prevColorTexture");
    mPrevDepthMapLocation = getUniformLocation("prevDepthMap");
    mCurrDepthMapLocation = getUniformLocation("currDepthMap");
    mMotionVectorsTextureLocation = getUniformLocation("motionVectorsTexture");
    mIsFirstFrame = getUniformLocation("isFirstFrame");
    mScreenSize = getUniformLocation("screenSize");
  }
}  // namespace RenderSystem

#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <functional>

#include "glad/glad.h"

namespace RenderSystem
{
  using GetShaderIV = std::function<void(GLuint, GLenum, GLint*)>;
  using GetShaderInfoLog = std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)>;
}  // namespace RenderSystem

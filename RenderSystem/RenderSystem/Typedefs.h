#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <functional>

#include <glm/glm.hpp>

namespace RenderSystem
{
    using GetShaderIV = std::function<void(GLuint, GLenum, GLint*)>;
    using GetShaderInfoLog = std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)>;
    using RGB = glm::vec3;
    using RGBA = glm::vec4;
}

#include "VertexArrayObject.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  VertexArrayObject::VertexArrayObject()
  {
    init();
  }

  VertexArrayObject::~VertexArrayObject()
  {
    glDeleteVertexArrays(1, &mVAO);
  }

  void VertexArrayObject::init()
  {
    glGenVertexArrays(1, &mVAO);
  }

  void VertexArrayObject::bind() const
  {
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &mSavedVAO);
    glBindVertexArray(mVAO);
  }

  void VertexArrayObject::unbind() const
  {
    glBindVertexArray(mSavedVAO);
  }
}

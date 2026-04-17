#include "VertexBufferObject.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  VertexBufferObject::VertexBufferObject()
  {
    init();
  }

  VertexBufferObject::~VertexBufferObject()
  {
    glDeleteBuffers(1, &mVBO);
  }

  void VertexBufferObject::init()
  {
    glGenBuffers(1, &mVBO);
  }

  void VertexBufferObject::bind() const
  {
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &mSavedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  }

  void VertexBufferObject::unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, mSavedVBO);
  }
}

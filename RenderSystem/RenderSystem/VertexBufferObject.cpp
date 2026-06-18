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
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  }
}

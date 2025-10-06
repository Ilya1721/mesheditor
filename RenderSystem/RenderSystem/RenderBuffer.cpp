#include "RenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "MeshCore/Constants.h"
#include "glad/glad.h"

namespace RenderSystem
{
  RenderBuffer::RenderBuffer() { init(); }

  RenderBuffer::~RenderBuffer()
  {
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
  }

  void RenderBuffer::init()
  {
    glGenBuffers(1, &mVBO);
    glGenVertexArrays(1, &mVAO);
  }

  void RenderBuffer::bind() const
  {
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &mSavedVAO);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &mSavedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindVertexArray(mVAO);
  }

  void RenderBuffer::unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, mSavedVBO);
    glBindVertexArray(mSavedVAO);
  }

  void RenderBuffer::loadRenderData(const RenderData& renderData)
  {
    const auto& compactData = renderData.getCompactData();
    glBufferData(
      GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(),
      GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      reinterpret_cast<void*>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      reinterpret_cast<void*>(6 * sizeof(float))
    );
  }
}  // namespace RenderSystem

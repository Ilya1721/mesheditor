#include "RenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Constants.h"
#include "glad/glad.h"

namespace RenderSystem
{
  RenderBuffer::RenderBuffer()
  {
    init();
  }

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
    constexpr auto stride = 20 * sizeof(float);
    const auto& compactData = renderData.getCompactData();
    glBufferData(
      GL_ARRAY_BUFFER, compactData.size() * sizeof(float), compactData.data(),
      GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float))
    );
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
      3, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(8 * sizeof(float))
    );
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
      4, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(12 * sizeof(float))
    );
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(
      5, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(16 * sizeof(float))
    );
  }

  void RenderBuffer::loadScreenQuadRenderData()
  {
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(SCREEN_QUAD_VERTICES), SCREEN_QUAD_VERTICES, GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
      reinterpret_cast<void*>(2 * sizeof(float))
    );
  }
}  // namespace RenderSystem

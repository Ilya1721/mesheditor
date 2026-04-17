#include "StaticQuadRenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  void StaticQuadRenderBuffer::loadRenderData(const std::vector<float>& renderData)
  {
    mVBO.bind();
    auto size = sizeof(float) * renderData.size();
    glBufferData(GL_ARRAY_BUFFER, size, renderData.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
      reinterpret_cast<void*>(2 * sizeof(float))
    );
    mVBO.unbind();
  }

  void StaticQuadRenderBuffer::bind() const
  {
    mVAO.bind();
  }

  void StaticQuadRenderBuffer::unbind() const
  {
    mVAO.unbind();
  }
}

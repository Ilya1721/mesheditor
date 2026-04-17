#include "MeshRenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  void MeshRenderBuffer::loadRenderData(const MeshRenderData& renderData)
  {
    mVBO.bind();
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
    mVBO.unbind();
  }

  void MeshRenderBuffer::bind() const
  {
    mVAO.bind();
  }

  void MeshRenderBuffer::unbind() const
  {
    mVAO.unbind();
  }
}

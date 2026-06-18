#include "MeshRenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  void MeshRenderBuffer::loadRenderData(const MeshRenderData& renderData)
  {
    bind();
    mVBO.bind();
    constexpr auto stride = 23 * sizeof(float);
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
      3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(8 * sizeof(float))
    );
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
      4, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(11 * sizeof(float))
    );
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(
      5, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(15 * sizeof(float))
    );
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(
      6, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(19 * sizeof(float))
    );
  }

  void MeshRenderBuffer::bind() const
  {
    mVAO.bind();
  }
}

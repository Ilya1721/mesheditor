#include "ParticlesRenderBuffer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"

namespace RenderSystem
{
  void ParticlesRenderBuffer::loadParticlesRenderData()
  {
    mParticlesVBO.bind();
    auto stride = 10 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, stride * MAX_PARTICLES, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
      3, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float))
    );
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
      4, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(4 * sizeof(float))
    );
    glVertexAttribDivisor(4, 1);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(
      5, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(float))
    );
    glVertexAttribDivisor(5, 1);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(
      6, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float))
    );
    glVertexAttribDivisor(6, 1);
    mParticlesVBO.unbind();
  }

  void ParticlesRenderBuffer::updateParticlesRenderData(
    const ParticlesRenderData& renderData
  )
  {
    mParticlesVBO.bind();
    const auto& compactData = renderData.getCompactData();
    glBufferSubData(
      GL_ARRAY_BUFFER, 0, compactData.size() * sizeof(float), compactData.data()
    );
    mParticlesVBO.unbind();
  }

  void ParticlesRenderBuffer::loadQuadRenderData(const std::vector<float>& renderData)
  {
    mQuadVBO.bind();
    auto size = sizeof(float) * renderData.size();
    glBufferData(GL_ARRAY_BUFFER, size, renderData.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
      reinterpret_cast<void*>(2 * sizeof(float))
    );
    mQuadVBO.unbind();
  }

  void ParticlesRenderBuffer::bind() const
  {
    mVAO.bind();
  }

  void ParticlesRenderBuffer::unbind() const
  {
    mVAO.unbind();
  }
}

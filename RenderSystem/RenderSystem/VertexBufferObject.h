#pragma once

namespace RenderSystem
{
  class VertexBufferObject
  {
   public:
    VertexBufferObject();
    VertexBufferObject(VertexBufferObject&&) = delete;
    ~VertexBufferObject();

    void bind() const;

   private:
    void init();

   protected:
    unsigned int mVBO;
  };
}

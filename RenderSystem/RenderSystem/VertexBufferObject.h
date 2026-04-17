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
    void unbind() const;

   private:
    void init();

   protected:
    unsigned int mVBO;
    mutable int mSavedVBO;
  };
}

#pragma once

namespace RenderSystem
{
  class VertexArrayObject
  {
   public:
    VertexArrayObject();
    VertexArrayObject(VertexArrayObject&&) = delete;
    ~VertexArrayObject();

    void bind() const;
    void unbind() const;

   private:
    void init();

   protected:
    unsigned int mVAO;
    mutable int mSavedVAO;
  };
}

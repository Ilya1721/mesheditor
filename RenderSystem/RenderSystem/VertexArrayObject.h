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

   private:
    void init();

   protected:
    unsigned int mVAO;
  };
}

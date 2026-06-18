#pragma once

#include <glm/glm.hpp>
#include <unordered_set>

namespace RenderSystem
{
  class Object3D;
  class RenderBuffer;
  class FrameBufferObject;
  class ShaderProgram;
  class Texture2D;
  struct Material;

  class Renderer
  {
   public:
    Renderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram
    );

    virtual ~Renderer() = default;
    virtual void render() const = 0;

   protected:
    void setup() const;

   protected:
    const RenderBuffer* mRenderBuffer;
    const FrameBufferObject* mFrameBufferObject;
    const ShaderProgram* mShaderProgram;
  };

  class Object3DRenderer : public Renderer
  {
   public:
    Object3DRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const Object3D* object,
      size_t vertexOffset
    );

    size_t getVertexOffset() const;
    void render() const override;

   protected:
    void setup() const;
    void setup(const glm::mat4& model, const Material& material) const;

   protected:
    const Object3D* mObject;
    size_t mVertexOffset;
  };

  class ObjectGlassRenderer : public Object3DRenderer
  {
   public:
    ObjectGlassRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const Object3D* object,
      size_t vertexOffset
    );

    void render() const override;
  };

  class ObjectShadowRenderer : public Object3DRenderer
  {
   public:
    ObjectShadowRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const Object3D* object,
      size_t vertexOffset
    );

    void render() const override;
  };

  class ObjectHighlightRenderer : public Object3DRenderer
  {
   public:
    ObjectHighlightRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const Object3D* object,
      size_t vertexOffset
    );

    void render() const override;
    void renderFace(size_t faceIdx) const;
  };

  class ObjectWireframeRenderer : public Object3DRenderer
  {
   public:
    ObjectWireframeRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const Object3D* object,
      size_t vertexOffset
    );

    void render() const override;
  };

  class ObjectMotionVectorsRenderer : public Object3DRenderer
  {
  public:
    ObjectMotionVectorsRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const Object3D* object,
      size_t vertexOffset
    );

    void render() const override;

  private:
    void setup() const;

  private:
    mutable glm::mat4 mPrevModel;
  };

  class QuadRenderer : public Renderer
  {
   public:
    QuadRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram
    );

    void render() const override;
  };

  class SkyboxRenderer : public Renderer
  {
   public:
    SkyboxRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram
    );

    void render() const override;
  };

  class ParticlesRenderer : public Renderer
  {
   public:
    ParticlesRenderer(
      const RenderBuffer* renderBuffer,
      const FrameBufferObject* frameBufferObject,
      const ShaderProgram* shaderProgram,
      const std::unordered_set<size_t>* activeParticlesIndices
    );

    void render() const override;

   private:
    const std::unordered_set<size_t>* mActiveParticlesIndices;
  };
}

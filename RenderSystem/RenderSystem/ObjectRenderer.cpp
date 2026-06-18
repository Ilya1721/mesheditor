#include "ObjectRenderer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"
#include "FrameBufferObject.h"
#include "Object3D.h"
#include "RenderBuffer.h"
#include "ShaderProgram.h"
#include "TAAMotionVectorsShaderProgram.h"

namespace RenderSystem
{
  Renderer::Renderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram
  )
    : mRenderBuffer(renderBuffer),
      mFrameBufferObject(frameBufferObject),
      mShaderProgram(shaderProgram)
  {
  }

  void Renderer::setup() const
  {
    mRenderBuffer->bind();
    mFrameBufferObject->bind();
    mShaderProgram->bind();
  }

  Object3DRenderer::Object3DRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const Object3D* object,
    size_t vertexOffset
  )
    : Renderer(renderBuffer, frameBufferObject, shaderProgram),
      mObject(object),
      mVertexOffset(vertexOffset)
  {
  }

  void Object3DRenderer::render() const
  {
    setup();
    glDrawArrays(mObject->getRenderMode(), mVertexOffset, mObject->getVertexCount());
  }

  size_t Object3DRenderer::getVertexOffset() const
  {
    return mVertexOffset;
  }

  void Object3DRenderer::setup() const
  {
    setup(mObject->getTransform(), mObject->getMaterial());
  }

  void Object3DRenderer::setup(const glm::mat4& model, const Material& material) const
  {
    Renderer::setup();
    auto shaderProgram = static_cast<const Object3DShaderProgram*>(mShaderProgram);
    shaderProgram->setModel(model);
    shaderProgram->setMaterial(material);
    mShaderProgram->preRenderSetup();
  }

  ObjectGlassRenderer::ObjectGlassRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const Object3D* object,
    size_t vertexOffset
  )
    : Object3DRenderer(
        renderBuffer, frameBufferObject, shaderProgram, object, vertexOffset
      )
  {
  }

  void ObjectGlassRenderer::render() const
  {
    setup();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDrawArrays(GL_TRIANGLES, mVertexOffset, mObject->getVertexCount());
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  }

  ObjectShadowRenderer::ObjectShadowRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const Object3D* object,
    size_t vertexOffset
  )
    : Object3DRenderer(
        renderBuffer, frameBufferObject, shaderProgram, object, vertexOffset
      )
  {
  }

  void ObjectShadowRenderer::render() const
  {
    setup();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, mVertexOffset, mObject->getVertexCount());
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
  }

  ObjectHighlightRenderer::ObjectHighlightRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const Object3D* object,
    size_t vertexOffset
  )
    : Object3DRenderer(
        renderBuffer, frameBufferObject, shaderProgram, object, vertexOffset
      )
  {
  }

  void ObjectHighlightRenderer::render() const
  {
    setup(mObject->getTransform(), RUBY_MATERIAL);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, mVertexOffset, mObject->getVertexCount());
    glDepthFunc(GL_LESS);
  }

  void ObjectHighlightRenderer::renderFace(size_t faceIdx) const
  {
    setup(mObject->getTransform(), RUBY_MATERIAL);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, faceIdx * 3 + mVertexOffset, 3);
    glDepthFunc(GL_LESS);
  }

  ObjectWireframeRenderer::ObjectWireframeRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const Object3D* object,
    size_t vertexOffset
  )
    : Object3DRenderer(
        renderBuffer, frameBufferObject, shaderProgram, object, vertexOffset
      )
  {
  }

  void ObjectWireframeRenderer::render() const
  {
    setup(mObject->getTransform(), BLACK_MATERIAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, mVertexOffset, mObject->getVertexCount());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  ObjectMotionVectorsRenderer::ObjectMotionVectorsRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const Object3D* object,
    size_t vertexOffset
  )
    : Object3DRenderer(
        renderBuffer, frameBufferObject, shaderProgram, object, vertexOffset
      ),
      mPrevModel(1.0f)
  {
  }

  void ObjectMotionVectorsRenderer::setup() const
  {
    Renderer::setup();
    auto shaderProgram =
      static_cast<const TAAMotionVectorsShaderProgram*>(mShaderProgram);
    const auto& currModel = mObject->getTransform();
    shaderProgram->setCurrModel(currModel);
    shaderProgram->setPrevModel(mPrevModel);
    mPrevModel = currModel;
  }

  void ObjectMotionVectorsRenderer::render() const
  {
    setup();
    glDrawArrays(mObject->getRenderMode(), mVertexOffset, mObject->getVertexCount());
  }

  QuadRenderer::QuadRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram
  )
    : Renderer(renderBuffer, frameBufferObject, shaderProgram)
  {
  }

  void QuadRenderer::render() const
  {
    Renderer::setup();
    mShaderProgram->preRenderSetup();
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
  }

  SkyboxRenderer::SkyboxRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram
  )
    : Renderer(renderBuffer, frameBufferObject, shaderProgram)
  {
  }

  void SkyboxRenderer::render() const
  {
    Renderer::setup();
    mShaderProgram->preRenderSetup();
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
  }

  ParticlesRenderer::ParticlesRenderer(
    const RenderBuffer* renderBuffer,
    const FrameBufferObject* frameBufferObject,
    const ShaderProgram* shaderProgram,
    const std::unordered_set<size_t>* activeParticlesIndices
  )
    : Renderer(renderBuffer, frameBufferObject, shaderProgram),
      mActiveParticlesIndices(activeParticlesIndices)
  {
  }

  void ParticlesRenderer::render() const
  {
    Renderer::setup();
    mShaderProgram->preRenderSetup();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mActiveParticlesIndices->size());
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
  }
}

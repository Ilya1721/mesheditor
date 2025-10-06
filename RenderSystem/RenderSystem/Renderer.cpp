#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/Mesh.h"
#include "Object3D.h"
#include "SceneShaderProgram.h"
#include "glad/glad.h"

using namespace MeshCore;

namespace RenderSystem
{
  Renderer::Renderer() : mModelRenderBuffer(), mDecorationsRenderBuffer() {}

  void Renderer::renderHighlightedFace(int faceIdx, int vertexOffset)
  {
    renderOverlayPrimitive([faceIdx, vertexOffset]()
                           { glDrawArrays(GL_TRIANGLES, faceIdx * 3 + vertexOffset, 3); }
    );
  }

  void Renderer::renderWireframe(int objectVertexCount)
  {
    renderOverlayPrimitive(
      [&objectVertexCount]()
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, objectVertexCount);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    );
  }

  void Renderer::renderWholeObjectHighlighted(const Object3D& object, int vertexOffset)
  {
    renderOverlayPrimitive([this, &object, vertexOffset]()
                           { renderObject3D(object, vertexOffset); });
  }

  void Renderer::renderSkybox()
  {
    mSkyboxRenderBuffer.invoke([]() { glDrawArrays(GL_TRIANGLES, 0, 36); });
  }

  void Renderer::renderObject3D(const Object3D& object, int vertexOffset)
  {
    mModelRenderBuffer.invoke(
      [&object, &vertexOffset]()
      { glDrawArrays(GL_TRIANGLES, vertexOffset, object.getVertexCount()); }
    );
  }

  void Renderer::renderOverlayPrimitive(const std::function<void()>& renderFunc)
  {
    glDepthFunc(GL_LEQUAL);
    renderFunc();
    glDepthFunc(GL_LESS);
  }

  void Renderer::cleanScreen() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

  void Renderer::loadDecorationsRenderData(const RenderData& renderData)
  {
    mDecorationsRenderBuffer.invoke(
      [this, &renderData]() { mDecorationsRenderBuffer.loadRenderData(renderData); }
    );
  }

  void Renderer::loadSkyboxRenderData(const RenderData& renderData)
  {
    mSkyboxRenderBuffer.invoke([this, &renderData]()
                               { mSkyboxRenderBuffer.loadRenderData(renderData); });
  }

  void Renderer::loadModelRenderData(const RenderData& renderData)
  {
    mModelRenderBuffer.invoke([this, &renderData]()
                              { mModelRenderBuffer.loadRenderData(renderData); });
  }

  void Renderer::renderSceneDecoration(
    const SceneDecoration& sceneDecoration, int& startIndex
  )
  {
    mDecorationsRenderBuffer.invoke(
      [&sceneDecoration, &startIndex]()
      {
        auto vertexCount = sceneDecoration.renderData.getVertexCount();
        glDrawArrays(sceneDecoration.renderMode, startIndex, vertexCount);
        startIndex += vertexCount;
      }
    );
  }
}  // namespace RenderSystem

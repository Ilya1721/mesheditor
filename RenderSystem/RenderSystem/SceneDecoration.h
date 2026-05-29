#pragma once

#include "Material.h"
#include "MeshCore/AABBox.h"
#include "MeshCore/BRepModel.h"
#include "MeshRenderData.h"

namespace GeometryCore
{
  struct Ray;
  struct Line;
  struct Plane;
}  // namespace GeometryCore

using namespace GeometryCore;

namespace RenderSystem
{
  struct SceneDecoration
  {
    MeshRenderData renderData;
    Material material;
    unsigned int renderMode {};

    void materialVisitor(
      const std::function<void(const BlinnPhongMaterial&)>& blinnPhongAction,
      const std::function<void(const ColorMaterial&)>& colorAction
    ) const;

    static SceneDecoration createSceneFloor(
      float sceneBBoxHeight, const Material& floorMaterial
    );
    static SceneDecoration createPlane(
      const glm::vec3& origin,
      const glm::vec3& normal,
      float width,
      float height,
      const Material& material
    );
    static SceneDecoration createBRepPlane(
      const glm::vec3& origin,
      const glm::vec3& normal,
      float width,
      float height,
      const Material& material
    );
    static SceneDecoration createLine(
      const glm::vec3& start,
      const glm::vec3& end,
      bool withArrowHead,
      const Material& material
    );
    static std::vector<SceneDecoration> createGlobalAxes(float length = 10.0f);
    static std::vector<SceneDecoration> createVerticesNormals(
      const std::vector<MeshCore::Vertex>& vertices
    );
    static std::vector<SceneDecoration> createBoundingBox(
      const MeshCore::AABBox& bbox, const Material& material
    );
    static std::vector<SceneDecoration> createPoints(
      const std::vector<glm::vec3>& points, const Material& material
    );
    static SceneDecoration createBRepCircle(
      const glm::vec3& center,
      const glm::vec3& normal,
      float radius,
      const Material& material
    );
    static SceneDecoration createCircle(
      const glm::vec3& center,
      const glm::vec3& normal,
      float radius,
      const Material& material
    );
    static SceneDecoration createBRepCylinder(
      const glm::vec3& origin,
      const glm::vec3& normal,
      float radius,
      float height,
      const Material& material
    );
    static SceneDecoration createBRepModel(
      const MeshCore::BRepModel& model, const Material& material
    );
  };
}  // namespace RenderSystem
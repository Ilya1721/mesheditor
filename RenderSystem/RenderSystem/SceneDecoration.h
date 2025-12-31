#pragma once

#include "GeometryCore/Typedefs.h"
#include "Material.h"
#include "MeshCore/AABBox.h"
#include "RenderData.h"

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
    RenderData renderData;
    BlinnPhongMaterial material;
    unsigned int renderMode;

    static SceneDecoration createDecoration(
      const Point3D& point, const BlinnPhongMaterial& material
    );
    static SceneDecoration createDecoration(
      const Ray& ray, float length, const BlinnPhongMaterial& material
    );
    static SceneDecoration createDecoration(
      const Line& line, bool withArrowHead, const BlinnPhongMaterial& material
    );
    static SceneDecoration createDecoration(
      const Plane& plane,
      float width,
      float length,
      const BlinnPhongMaterial& material
    );

    static SceneDecoration createSceneFloor(
      float sceneBBoxHeight, const BlinnPhongMaterial& floorBlinnPhongMaterial
    );
    static SceneDecoration createPlane(
      const Point3D& origin,
      const Vector3D& normal,
      float width,
      float height,
      const BlinnPhongMaterial& material
    );
    static SceneDecoration createLine(
      const Point3D& start,
      const Point3D& end,
      bool withArrowHead,
      const BlinnPhongMaterial& material
    );
    static std::vector<SceneDecoration> createGlobalAxes(float length = 10.0f);
    static std::vector<SceneDecoration> createVerticesNormals(
      const std::vector<MeshCore::Vertex>& vertices
    );
    static std::vector<SceneDecoration> createBoundingBox(
      const MeshCore::AABBox& bbox, const BlinnPhongMaterial& material
    );
    static std::vector<SceneDecoration> createPoints(
      const std::vector<Point3D>& points, const BlinnPhongMaterial& material
    );
  };
}  // namespace RenderSystem
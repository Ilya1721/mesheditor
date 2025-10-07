#pragma once

#include "GeometryCore/Typedefs.h"
#include "MeshCore/AABBox.h"
#include "MeshCore/MaterialParams.h"
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
    BlinnPhongMaterialParams materialParams {};
    unsigned int renderMode {};

    static SceneDecoration createDecoration(
      const Ray& ray, float length, const BlinnPhongMaterialParams& materialParams
    );
    static SceneDecoration createDecoration(
      const Line& line, bool withArrowHead, const BlinnPhongMaterialParams& materialParams
    );
    static SceneDecoration createDecoration(
      const Plane& plane,
      float width,
      float length,
      const BlinnPhongMaterialParams& materialParams
    );

    static SceneDecoration createSceneFloor(
      float sceneBBoxHeight, const BlinnPhongMaterialParams& floorBlinnPhongMaterialParams
    );
    static SceneDecoration createPlane(
      const Point3D& origin,
      const Vector3D& normal,
      float width,
      float height,
      const BlinnPhongMaterialParams& materialParams
    );
    static SceneDecoration createLine(
      const Point3D& start,
      const Point3D& end,
      bool withArrowHead,
      const BlinnPhongMaterialParams& materialParams
    );
    static std::vector<SceneDecoration> createGlobalAxes(float length = 10.0f);
    static std::vector<SceneDecoration> createVerticesNormals(
      const std::vector<MeshCore::Vertex>& vertices
    );
    static std::vector<SceneDecoration> createBoundingBox(
      const MeshCore::AABBox& bbox, const BlinnPhongMaterialParams& materialParams
    );
  };
}  // namespace RenderSystem
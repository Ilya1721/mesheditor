#pragma once

#include "GeometryCore/Typedefs.h"
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
    MaterialParams materialParams {};
    unsigned int renderMode {};

    static SceneDecoration createDecoration(
      const Ray& ray, float length, const MaterialParams& materialParams
    );
    static SceneDecoration createDecoration(
      const Line& line, bool withArrowHead, const MaterialParams& materialParams
    );
    static SceneDecoration createDecoration(
      const Plane& plane, float width, float length, const MaterialParams& materialParams
    );

    static SceneDecoration createSceneFloor(
      float sceneBBoxHeight, const MaterialParams& floorMaterialParams
    );
    static SceneDecoration createPlane(
      const Point3D& origin,
      const Vector3D& normal,
      float width,
      float height,
      const MaterialParams& materialParams
    );
    static SceneDecoration createLine(
      const Point3D& start, const Point3D& end, const MaterialParams& materialParams
    );
    static std::vector<SceneDecoration> createGlobalAxes(float length = 10.0f);
    static std::vector<SceneDecoration> createVerticesNormals(
      const std::vector<MeshCore::Vertex>& vertices
    );
  };
}  // namespace RenderSystem
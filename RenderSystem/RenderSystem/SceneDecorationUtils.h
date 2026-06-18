#pragma once

#include "MeshCore/AABBox.h"
#include "MeshCore/BRepModel.h"
#include "Object3D.h"

using namespace GeometryCore;

namespace RenderSystem
{
  std::unique_ptr<Object3D> createSceneFloor(
    float sceneBBoxHeight, const Material& floorMaterial
  );
  std::unique_ptr<Object3D> createPlane(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float width,
    float height,
    const Material& material
  );
  std::unique_ptr<Object3D> createBRepPlane(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float width,
    float height,
    const Material& material
  );
  std::unique_ptr<Object3D> createLine(
    const glm::vec3& start,
    const glm::vec3& end,
    bool withArrowHead,
    const Material& material
  );
  std::vector<std::unique_ptr<Object3D>> createGlobalAxes(float length = 10.0f);
  std::vector<std::unique_ptr<Object3D>> createVerticesNormals(
    const std::vector<MeshCore::Vertex>& vertices
  );
  std::vector<std::unique_ptr<Object3D>> createBoundingBox(
    const MeshCore::AABBox& bbox, const Material& material
  );
  std::vector<std::unique_ptr<Object3D>> createPoints(
    const std::vector<glm::vec3>& points, const Material& material
  );
  std::unique_ptr<Object3D> createBRepCircle(
    const glm::vec3& center,
    const glm::vec3& normal,
    float radius,
    const Material& material
  );
  std::unique_ptr<Object3D> createCircle(
    const glm::vec3& center,
    const glm::vec3& normal,
    float radius,
    const Material& material
  );
  std::unique_ptr<Object3D> createBRepCylinder(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float radius,
    float height,
    const Material& material
  );
  std::unique_ptr<Object3D> createBRepModel(
    const MeshCore::BRepModel& model, const Material& material
  );
}  // namespace RenderSystem
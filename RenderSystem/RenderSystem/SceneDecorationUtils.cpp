#include "SceneDecorationUtils.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"
#include "GeometryCore/Line.h"
#include "MeshCore/BRepFactory.h"
#include "MeshCore/MeshFactory.h"
#include "MeshCore/TransformUtils.h"
#include "MeshCore/Vertex.h"

namespace RenderSystem
{
  std::unique_ptr<Object3D> createSceneFloor(
    float sceneBBoxHeight, const Material& floorMaterial
  )
  {
    auto originY = -sceneBBoxHeight * FLOOR_BBOX_HEIGHT_COEF;
    glm::vec3 planeOrigin(0.0f, originY, 0.0f);
    glm::vec3 planeNormal(0.0f, -1.0f, 0.0f);

    return createPlane(
      planeOrigin, planeNormal, FAR_PLANE_DISTANCE * 0.5, FAR_PLANE_DISTANCE * 0.5,
      floorMaterial
    );
  }

  std::unique_ptr<Object3D> createPlane(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float width,
    float height,
    const Material& material
  )
  {
    auto planeVertices = MeshCore::createUnitXYPlane();
    auto transform = MeshCore::getUnitXYPlaneTransform(origin, normal, width, height);
    auto mesh = std::make_unique<Mesh>(planeVertices);

    return std::make_unique<Object3D>(std::move(mesh), material, transform, GL_TRIANGLES);
  }

  std::unique_ptr<Object3D> createBRepPlane(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float width,
    float height,
    const Material& material
  )
  {
    auto brepPlane = MeshCore::createUnitXYBRepPlane();
    auto planeVertices = MeshCore::getBRepSurfaceVertices(brepPlane, 16, 16);
    auto transform = MeshCore::getUnitXYPlaneTransform(origin, normal, width, height);
    auto mesh = std::make_unique<Mesh>(planeVertices);

    return std::make_unique<Object3D>(std::move(mesh), material, transform, GL_TRIANGLES);
  }

  std::unique_ptr<Object3D> createLine(
    const glm::vec3& start,
    const glm::vec3& end,
    bool withArrowHead,
    const Material& material
  )
  {
    auto unitXLineVertices = MeshCore::createUnitXLine(withArrowHead);
    auto transform = MeshCore::getUnitXLineTransform(start, end);
    auto mesh = std::make_unique<Mesh>(unitXLineVertices, false);

    return std::make_unique<Object3D>(std::move(mesh), material, transform, GL_LINES);
  }

  std::vector<std::unique_ptr<Object3D>> createGlobalAxes(float length)
  {
    std::vector<std::unique_ptr<Object3D>> globalAxes;
    globalAxes.emplace_back(createLine(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(length, 0.0f, 0.0f), true, BLUE_MATERIAL
    ));
    globalAxes.emplace_back(createLine(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, length, 0.0f), true, RED_MATERIAL
    ));
    globalAxes.emplace_back(createLine(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, length), true, GREEN_MATERIAL
    ));

    return globalAxes;
  }

  std::vector<std::unique_ptr<Object3D>> createVerticesNormals(
    const std::vector<MeshCore::Vertex>& vertices
  )
  {
    std::vector<std::unique_ptr<Object3D>> verticesNormals;
    for (const auto& vertex : vertices)
    {
      verticesNormals.emplace_back(
        createLine(vertex.pos, vertex.pos + vertex.normal * 10.0f, true, GREEN_MATERIAL)
      );
    }

    return verticesNormals;
  }

  std::vector<std::unique_ptr<Object3D>> createBoundingBox(
    const MeshCore::AABBox& bbox, const Material& material
  )
  {
    glm::vec3 leftMinLowerCorner = bbox.getMin();
    glm::vec3 rightMinLowerCorner = {
      bbox.getMax().x, leftMinLowerCorner.y, leftMinLowerCorner.z
    };
    glm::vec3 leftMaxLowerCorner = {
      leftMinLowerCorner.x, leftMinLowerCorner.y, bbox.getMax().z
    };
    glm::vec3 rightMaxLowerCorner = {
      rightMinLowerCorner.x, rightMinLowerCorner.y, bbox.getMax().z
    };
    glm::vec3 leftMinUpperCorner = {
      leftMinLowerCorner.x, bbox.getMax().y, leftMinLowerCorner.z
    };
    glm::vec3 rightMinUpperCorner = {
      rightMinLowerCorner.x, bbox.getMax().y, rightMinLowerCorner.z
    };
    glm::vec3 leftMaxUpperCorner = {
      leftMaxLowerCorner.x, bbox.getMax().y, leftMaxLowerCorner.z
    };
    glm::vec3 rightMaxUpperCorner = {
      rightMaxLowerCorner.x, bbox.getMax().y, rightMaxLowerCorner.z
    };

    std::vector<std::unique_ptr<Object3D>> bboxLines;
    bboxLines.push_back(
      createLine(leftMinLowerCorner, rightMinLowerCorner, false, material)
    );
    bboxLines.push_back(
      createLine(rightMinLowerCorner, rightMaxLowerCorner, false, material)
    );
    bboxLines.push_back(
      createLine(rightMaxLowerCorner, leftMaxLowerCorner, false, material)
    );
    bboxLines.push_back(
      createLine(leftMaxLowerCorner, leftMinLowerCorner, false, material)
    );
    bboxLines.push_back(
      createLine(leftMinUpperCorner, rightMinUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(rightMinUpperCorner, rightMaxUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(rightMaxUpperCorner, leftMaxUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(leftMaxUpperCorner, leftMinUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(leftMinLowerCorner, leftMinUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(rightMinLowerCorner, rightMinUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(rightMaxLowerCorner, rightMaxUpperCorner, false, material)
    );
    bboxLines.push_back(
      createLine(leftMaxLowerCorner, leftMaxUpperCorner, false, material)
    );

    return bboxLines;
  }

  std::vector<std::unique_ptr<Object3D>> createPoints(
    const std::vector<glm::vec3>& points, const Material& material
  )
  {
    std::vector<std::unique_ptr<Object3D>> pointDecorations;
    for (const auto& point : points)
    {
      Vertex vertex(point, glm::vec3(0.0f));
      auto mesh = std::make_unique<Mesh>(std::vector<Vertex> {vertex});
      pointDecorations.push_back(std::make_unique<Object3D>(std::move(mesh), material));
    }

    return pointDecorations;
  }

  std::unique_ptr<Object3D> createBRepCircle(
    const glm::vec3& center,
    const glm::vec3& normal,
    float radius,
    const Material& material
  )
  {
    auto brepCircle = MeshCore::createUnitXYBRepCircle();
    auto circleVertices = MeshCore::getBRepCurveVertices(brepCircle, 32);
    auto transform = MeshCore::getUnitXYCircleTransform(center, normal, radius);
    auto mesh = std::make_unique<Mesh>(circleVertices, false);

    return std::make_unique<Object3D>(
      std::move(mesh), material, transform, GL_LINE_STRIP
    );
  }

  std::unique_ptr<Object3D> createCircle(
    const glm::vec3& center,
    const glm::vec3& normal,
    float radius,
    const Material& material
  )
  {
    auto circleVertices = MeshCore::createUnitXYCircle(32);
    auto transform = MeshCore::getUnitXYCircleTransform(center, normal, radius);
    auto mesh = std::make_unique<Mesh>(circleVertices, false);

    return std::make_unique<Object3D>(
      std::move(mesh), material, transform, GL_LINE_STRIP
    );
  }

  std::unique_ptr<Object3D> createBRepCylinder(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float radius,
    float height,
    const Material& material
  )
  {
    auto brepCylinder = MeshCore::createUnitBRepCylinder();
    auto cylinderVertices = MeshCore::getBRepSurfaceVertices(brepCylinder, 32, 32);
    auto transform = MeshCore::getUnitCylinderTransform(origin, normal, radius, height);
    auto mesh = std::make_unique<Mesh>(cylinderVertices, false);

    return std::make_unique<Object3D>(std::move(mesh), material, transform, GL_TRIANGLES);
  }

  std::unique_ptr<Object3D> createBRepModel(
    const MeshCore::BRepModel& model, const Material& material
  )
  {
    auto vertices = MeshCore::getBRepModelVertices(model);
    auto mesh = std::make_unique<Mesh>(vertices, false);
    return std::make_unique<Object3D>(std::move(mesh), material);
  }
}  // namespace RenderSystem
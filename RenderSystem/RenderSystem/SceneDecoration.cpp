#include "SceneDecoration.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Constants.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "MeshCore/Vertex.h"
#include "glad/glad.h"

using namespace MeshCore;

namespace
{
  using namespace RenderSystem;

  SceneDecoration getBaseSceneDecoration(
    const BlinnPhongMaterialParams& materialParams,
    int renderMode,
    const RenderData& renderData
  )
  {
    SceneDecoration primitive;
    primitive.materialParams = materialParams;
    primitive.renderMode = renderMode;
    primitive.renderData = renderData;

    return primitive;
  }
}  // namespace

namespace RenderSystem
{
  SceneDecoration SceneDecoration::createDecoration(
    const GeometryCore::Ray& ray,
    float length,
    const BlinnPhongMaterialParams& materialParams
  )
  {
    return getBaseSceneDecoration(
      materialParams, GL_LINES, RenderData::createRenderData(ray, length)
    );
  }

  SceneDecoration SceneDecoration::createDecoration(
    const GeometryCore::Line& line,
    bool withArrowHead,
    const BlinnPhongMaterialParams& materialParams
  )
  {
    return getBaseSceneDecoration(
      materialParams, GL_LINES, RenderData::createRenderData(line, withArrowHead)
    );
  }

  SceneDecoration SceneDecoration::createDecoration(
    const GeometryCore::Plane& plane,
    float width,
    float length,
    const BlinnPhongMaterialParams& materialParams
  )
  {
    return getBaseSceneDecoration(
      materialParams, GL_TRIANGLES, RenderData::createRenderData(plane, width, length)
    );
  }

  SceneDecoration SceneDecoration::createSceneFloor(
    float sceneBBoxHeight, const BlinnPhongMaterialParams& floorMaterialParams
  )
  {
    auto originY = -sceneBBoxHeight * FLOOR_BBOX_HEIGHT_COEF;
    Point3D planeOrigin(0.0f, originY, 0.0f);
    Vector3D planeNormal(0.0f, -1.0f, 0.0f);

    return createPlane(
      planeOrigin, planeNormal, FAR_PLANE_DISTANCE * 0.5, FAR_PLANE_DISTANCE * 0.5,
      floorMaterialParams
    );
  }

  SceneDecoration SceneDecoration::createPlane(
    const Point3D& origin,
    const Vector3D& normal,
    float width,
    float height,
    const BlinnPhongMaterialParams& materialParams
  )
  {
    return SceneDecoration::createDecoration(
      {origin, normal}, width, height, materialParams
    );
  }

  SceneDecoration SceneDecoration::createLine(
    const Point3D& start,
    const Point3D& end,
    bool withArrowHead,
    const BlinnPhongMaterialParams& material
  )
  {
    return SceneDecoration::createDecoration({start, end}, withArrowHead, material);
  }

  std::vector<SceneDecoration> SceneDecoration::createGlobalAxes(float length)
  {
    std::vector<SceneDecoration> globalAxes;
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

  std::vector<SceneDecoration> SceneDecoration::createVerticesNormals(
    const std::vector<MeshCore::Vertex>& vertices
  )
  {
    std::vector<SceneDecoration> verticesNormals;

    for (const auto& vertex : vertices)
    {
      verticesNormals.emplace_back(
        createLine(vertex.pos, vertex.pos + vertex.normal * 10.0f, true, GREEN_MATERIAL)
      );
    }

    return verticesNormals;
  }

  std::vector<SceneDecoration> SceneDecoration::createBoundingBox(
    const MeshCore::AABBox& bbox, const BlinnPhongMaterialParams& materialParams
  )
  {
    Point3D leftMinLowerCorner = bbox.getMin();
    Point3D rightMinLowerCorner = {
      bbox.getMax().x, leftMinLowerCorner.y, leftMinLowerCorner.z
    };
    Point3D leftMaxLowerCorner = {
      leftMinLowerCorner.x, leftMinLowerCorner.y, bbox.getMax().z
    };
    Point3D rightMaxLowerCorner = {
      rightMinLowerCorner.x, rightMinLowerCorner.y, bbox.getMax().z
    };
    Point3D leftMinUpperCorner = {
      leftMinLowerCorner.x, bbox.getMax().y, leftMinLowerCorner.z
    };
    Point3D rightMinUpperCorner = {
      rightMinLowerCorner.x, bbox.getMax().y, rightMinLowerCorner.z
    };
    Point3D leftMaxUpperCorner = {
      leftMaxLowerCorner.x, bbox.getMax().y, leftMaxLowerCorner.z
    };
    Point3D rightMaxUpperCorner = {
      rightMaxLowerCorner.x, bbox.getMax().y, rightMaxLowerCorner.z
    };

    std::vector<SceneDecoration> bboxLines;
    bboxLines.push_back(
      createLine(leftMinLowerCorner, rightMinLowerCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(rightMinLowerCorner, rightMaxLowerCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(rightMaxLowerCorner, leftMaxLowerCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(leftMaxLowerCorner, leftMinLowerCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(leftMinUpperCorner, rightMinUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(rightMinUpperCorner, rightMaxUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(rightMaxUpperCorner, leftMaxUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(leftMaxUpperCorner, leftMinUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(leftMinLowerCorner, leftMinUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(rightMinLowerCorner, rightMinUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(rightMaxLowerCorner, rightMaxUpperCorner, false, materialParams)
    );
    bboxLines.push_back(
      createLine(leftMaxLowerCorner, leftMaxUpperCorner, false, materialParams)
    );

    return bboxLines;
  }
}  // namespace RenderSystem
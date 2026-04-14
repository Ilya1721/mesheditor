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
    const BlinnPhongMaterial& material, int renderMode, const RenderData& renderData
  )
  {
    SceneDecoration primitive;
    primitive.material = material;
    primitive.renderMode = renderMode;
    primitive.renderData = renderData;

    return primitive;
  }
}  // namespace

namespace RenderSystem
{
  SceneDecoration SceneDecoration::createDecoration(
    const glm::vec3& point, const BlinnPhongMaterial& params
  )
  {
    return getBaseSceneDecoration(params, GL_POINTS, RenderData::createRenderData(point));
  }

  SceneDecoration SceneDecoration::createDecoration(
    const GeometryCore::Ray& ray, float length, const BlinnPhongMaterial& material
  )
  {
    return getBaseSceneDecoration(
      material, GL_LINES, RenderData::createRenderData(ray, length)
    );
  }

  SceneDecoration SceneDecoration::createDecoration(
    const GeometryCore::Line& line, bool withArrowHead, const BlinnPhongMaterial& material
  )
  {
    return getBaseSceneDecoration(
      material, GL_LINES, RenderData::createRenderData(line, withArrowHead)
    );
  }

  SceneDecoration SceneDecoration::createDecoration(
    const GeometryCore::Plane& plane,
    float width,
    float length,
    const BlinnPhongMaterial& material
  )
  {
    return getBaseSceneDecoration(
      material, GL_TRIANGLES, RenderData::createRenderData(plane, width, length)
    );
  }

  SceneDecoration SceneDecoration::createSceneFloor(
    float sceneBBoxHeight, const BlinnPhongMaterial& floorMaterial
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

  SceneDecoration SceneDecoration::createPlane(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float width,
    float height,
    const BlinnPhongMaterial& material
  )
  {
    return SceneDecoration::createDecoration({origin, normal}, width, height, material);
  }

  SceneDecoration SceneDecoration::createLine(
    const glm::vec3& start,
    const glm::vec3& end,
    bool withArrowHead,
    const BlinnPhongMaterial& material
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
    const MeshCore::AABBox& bbox, const BlinnPhongMaterial& material
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

    std::vector<SceneDecoration> bboxLines;
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

  std::vector<SceneDecoration> SceneDecoration::createPoints(
    const std::vector<glm::vec3>& points, const BlinnPhongMaterial& material
  )
  {
    std::vector<SceneDecoration> pointDecorations;
    for (const auto& point : points)
    {
      pointDecorations.emplace_back(createDecoration(point, material));
    }

    return pointDecorations;
  }
}  // namespace RenderSystem
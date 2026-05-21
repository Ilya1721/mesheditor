#include "SceneDecoration.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/BRepFactory.h"
#include "MeshCore/MeshFactory.h"
#include "MeshCore/Vertex.h"

namespace
{
  using namespace RenderSystem;

  SceneDecoration createBaseSceneDecoration(
    const Material& material, int renderMode, const MeshRenderData& renderData
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
  void SceneDecoration::materialVisitor(
    const std::function<void(const BlinnPhongMaterial&)>& blinnPhongAction,
    const std::function<void(const ColorMaterial&)>& colorAction
  ) const
  {
    std::visit(
      [&](auto&& material)
      {
        using MaterialType = std::decay_t<decltype(material)>;
        if constexpr (std::is_same_v<MaterialType, BlinnPhongMaterial>)
        {
          blinnPhongAction(material);
        }
        else if constexpr (std::is_same_v<MaterialType, ColorMaterial>)
        {
          colorAction(material);
        }
      },
      material
    );
  }

  SceneDecoration SceneDecoration::createSceneFloor(
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

  SceneDecoration SceneDecoration::createPlane(
    const glm::vec3& origin,
    const glm::vec3& normal,
    float width,
    float height,
    const Material& material
  )
  {
    GeometryCore::Plane plane(origin, normal);
    auto planeVertices = MeshCore::createPlane(plane, width, height);
    auto renderData = MeshRenderData::generateRenderData(planeVertices);

    return createBaseSceneDecoration(material, GL_TRIANGLES, renderData);
  }

  SceneDecoration SceneDecoration::createLine(
    const glm::vec3& start,
    const glm::vec3& end,
    bool withArrowHead,
    const Material& material
  )
  {
    GeometryCore::Line line {start, end};
    auto lineVertices = MeshCore::createLine(line, withArrowHead);
    auto renderData = MeshRenderData::generateRenderData(lineVertices);

    return createBaseSceneDecoration(material, GL_LINES, renderData);
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
    const std::vector<glm::vec3>& points, const Material& material
  )
  {
    std::vector<SceneDecoration> pointDecorations;
    for (const auto& point : points)
    {
      Vertex vertex(point, glm::vec3(0.0f));
      auto renderData = MeshRenderData::generateRenderData({vertex});
      pointDecorations.emplace_back(
        createBaseSceneDecoration(material, GL_POINTS, renderData)
      );
    }

    return pointDecorations;
  }

  SceneDecoration SceneDecoration::createBRepCircle(
    float radius, const glm::vec3& normal, const Material& material
  )
  {
    auto brepCircle = MeshCore::createBRepCircle(radius, normal);
    auto circleVertices = MeshCore::getBRepCurveVertices(brepCircle, 12);
    auto renderData = MeshRenderData::generateRenderData(circleVertices);

    return createBaseSceneDecoration(material, GL_LINES, renderData);
  }
}  // namespace RenderSystem
#include "SceneDecoration.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"
#include "MeshCore/Vertex.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"

#include "Constants.h"

using namespace MeshCore;

namespace
{
    using namespace RenderSystem;

    SceneDecoration getBaseSceneDecoration(const Material& material, int renderMode, const RenderData& renderData)
    {
        SceneDecoration primitive;
        primitive.material = material;
        primitive.renderMode = renderMode;
        primitive.renderData = renderData;

        return primitive;
    }
}

namespace RenderSystem
{
    SceneDecoration SceneDecoration::createDecoration(const GeometryCore::Ray& ray, float length, const Material& material)
    {
        return getBaseSceneDecoration(material, GL_LINES, RenderData::createRenderData(ray, length));
    }

    SceneDecoration SceneDecoration::createDecoration(const GeometryCore::Line& line, bool withArrowHead, const Material& material)
    {
        return getBaseSceneDecoration(material, GL_LINES, RenderData::createRenderData(line, withArrowHead));
    }

    SceneDecoration SceneDecoration::createDecoration(const GeometryCore::Plane& plane, float width, float length, const Material& material)
    {
        return getBaseSceneDecoration(material, GL_TRIANGLES, RenderData::createRenderData(plane, width, length));
    }

    SceneDecoration SceneDecoration::createSceneFloor(float sceneBBoxHeight, const Material& floorMaterial)
    {
        auto originY = -sceneBBoxHeight * FLOOR_BBOX_HEIGHT_COEF;
        Point3D planeOrigin(0.0f, originY, 0.0f);
        Vector3D planeNormal(0.0f, -1.0f, 0.0f);

        return createPlane(planeOrigin, planeNormal, FAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE, floorMaterial);
    }

    SceneDecoration SceneDecoration::createPlane(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material)
    {
        return SceneDecoration::createDecoration({ origin, normal }, width, height, material);
    }

    SceneDecoration SceneDecoration::createLine(const Point3D& start, const Point3D& end, const Material& material)
    {
        return SceneDecoration::createDecoration({ start, end }, true, material);
    }

    std::vector<SceneDecoration> SceneDecoration::createGlobalAxes(float length)
    {
        std::vector<SceneDecoration> globalAxes;
        globalAxes.emplace_back(createLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(length, 0.0f, 0.0f), BLUE_MATERIAL));
        globalAxes.emplace_back(createLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, length, 0.0f), RED_MATERIAL));
        globalAxes.emplace_back(createLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, length), GREEN_MATERIAL));

        return globalAxes;
    }

    std::vector<SceneDecoration> SceneDecoration::createVerticesNormals(const std::vector<MeshCore::Vertex>& vertices)
    {
        std::vector<SceneDecoration> verticesNormals;

        for (const auto& vertex : vertices)
        {
            verticesNormals.emplace_back(createLine(vertex.pos, vertex.pos + vertex.normal * 10.0f, GREEN_MATERIAL));
        }

        return verticesNormals;
    }
}
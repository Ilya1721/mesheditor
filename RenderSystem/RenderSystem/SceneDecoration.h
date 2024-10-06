#pragma once

#include "GeometryCore/Typedefs.h"

#include "RenderData.h"
#include "Material.h"

namespace GeometryCore
{
    struct Ray;
    struct Line;
    struct Plane;
}

using namespace GeometryCore;

namespace RenderSystem
{
    struct SceneDecoration
    {
        RenderData renderData;
        Material material{};
        unsigned int renderMode{};

        static SceneDecoration createDecoration(const Ray& ray, float length, const Material& material);
        static SceneDecoration createDecoration(const Line& line, bool withArrowHead, const Material& material);
        static SceneDecoration createDecoration(const Plane& plane, float width, float length, const Material& material);

        static SceneDecoration createSceneFloor(float sceneBBoxHeight, const Material& floorMaterial);
        static SceneDecoration createPlane(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material);
        static SceneDecoration createLine(const Point3D& start, const Point3D& end, const Material& material);
        static std::vector<SceneDecoration> createGlobalAxes(float length = 10.0f);
        static std::vector<SceneDecoration> createVerticesNormals(const std::vector<MeshCore::Vertex>& vertices);
    };
}
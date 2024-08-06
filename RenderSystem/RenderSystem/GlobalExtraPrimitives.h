#pragma once

#include "GeometryCore/Typedefs.h"

#include "RenderPrimitive.h"
#include "Material.h"

using namespace GeometryCore;

namespace RenderSystem
{
    class GlobalExtraPrimitives
    {
    public:
        ~GlobalExtraPrimitives() = delete;

        static const std::vector<RenderPrimitive>& getExtraPrimitives();

        static void addSceneFloor();
        static void add(const RenderPrimitive& primitive);
        static void addPlane(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material);
        static void addLine(const Point3D& start, const Point3D& end, const Material& material);
        static void addGlobalAxes(float length = 10.0f);
        static void addVerticesNormals(const std::vector<MeshCore::Vertex>& vertices);

    private:
        static std::vector<RenderPrimitive> gExtraPrimitives;
    };
}

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
        static GlobalExtraPrimitives& getInstance();

        const std::vector<RenderPrimitive>& getExtraPrimitives();

        void addSceneFloor();
        void add(const RenderPrimitive& primitive);
        void addPlane(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material);
        void addLine(const Point3D& start, const Point3D& end, const Material& material);
        void addGlobalAxes(float length = 10.0f);
        void addVerticesNormals(const std::vector<MeshCore::Vertex>& vertices);

    private:
        GlobalExtraPrimitives();
        ~GlobalExtraPrimitives() = default;

    private:
        std::vector<RenderPrimitive> mExtraPrimitives;
    };
}

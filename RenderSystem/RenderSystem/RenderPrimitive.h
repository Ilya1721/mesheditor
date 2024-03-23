#pragma once

#include "MeshCore/RenderData.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"

#include "Material.h"

namespace RenderSystem
{
    struct RenderPrimitive
    {
        MeshCore::RenderData renderData;
        Material material;
        unsigned int renderMode;

        int getVertexCount() const;

        static RenderPrimitive createPrimitive(const GeometryCore::Ray& ray, float length, const Material& material);
        static RenderPrimitive createPrimitive(const GeometryCore::Line& line, bool withArrowHead, const Material& material);
        static RenderPrimitive createPrimitive(const GeometryCore::Plane& ray, float width, float length, const Material& material);
    };
}
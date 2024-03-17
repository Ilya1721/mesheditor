#pragma once

#include "MeshCore/RenderData.h"
#include "GeometryCore/Ray.h"

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
    };
}
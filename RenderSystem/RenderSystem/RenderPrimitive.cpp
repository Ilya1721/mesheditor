#include "RenderPrimitive.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"

using namespace MeshCore;

namespace
{
    using namespace RenderSystem;

    RenderPrimitive getBaseRenderPrimitive(const Material& material, int renderMode, const RenderData& renderData)
    {
        RenderPrimitive primitive;
        primitive.material = material;
        primitive.renderMode = renderMode;
        primitive.renderData = renderData;

        return primitive;
    }
}

namespace RenderSystem
{
    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Ray& ray, float length, const Material& material)
    {
        return getBaseRenderPrimitive(material, GL_LINES, RenderData::createRenderData(ray, length));
    }

    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Line& line, bool withArrowHead, const Material& material)
    {
        return getBaseRenderPrimitive(material, GL_LINES, RenderData::createRenderData(line, withArrowHead));
    }

    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Plane& plane, float width, float length, const Material& material)
    {
        return getBaseRenderPrimitive(material, GL_TRIANGLES, RenderData::createRenderData(plane, width, length));
    }
}
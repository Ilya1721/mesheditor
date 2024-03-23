#include "RenderPrimitive.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"

namespace
{
    using namespace RenderSystem;

    RenderPrimitive getBaseRenderPrimitive(const Material& material)
    {
        RenderPrimitive primitive;
        primitive.material = material;

        return primitive;
    }
}

namespace RenderSystem
{
    int RenderPrimitive::getVertexCount() const
    {
        return renderData.positions.size() / MeshCore::COORDINATES_PER_VERTEX;
    }

    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Ray& ray, float length, const Material& material)
    {
        auto primitive = getBaseRenderPrimitive(material);
        primitive.renderData = MeshCore::RenderData::createRenderData(ray, length);
        primitive.renderMode = GL_LINES;

        return primitive;
    }

    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Line& line, bool withArrowHead, const Material& material)
    {
        auto primitive = getBaseRenderPrimitive(material);
        primitive.renderData = MeshCore::RenderData::createRenderData(line, withArrowHead);
        primitive.renderMode = GL_LINES;

        return primitive;
    }

    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Plane& plane, float width, float length, const Material& material)
    {
        auto primitive = getBaseRenderPrimitive(material);
        primitive.renderData = MeshCore::RenderData::createRenderData(plane, width, length);
        primitive.renderMode = GL_TRIANGLES;

        return primitive;
    }
}
#include "RenderPrimitive.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "MeshCore/Constants.h"

namespace RenderSystem
{
    int RenderPrimitive::getVertexCount() const
    {
        return renderData.positions.size() / MeshCore::COORDINATES_PER_VERTEX;
    }

    RenderPrimitive RenderPrimitive::createPrimitive(const GeometryCore::Ray& ray, float length, const Material& material)
    {
        RenderPrimitive primitive;
        primitive.renderData = MeshCore::RenderData::createRenderData(ray, length);
        primitive.material = material;
        primitive.renderMode = GL_LINES;

        return primitive;
    }
}
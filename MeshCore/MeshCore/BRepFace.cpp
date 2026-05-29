#include "BRepFace.h"

#include "Constants.h"

namespace MeshCore
{
  SurfaceDomain BRepFace::getDomain() const
  {
    SurfaceDomain faceDomain {
      std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(),
      std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()
    };

    auto startHalfEdge = outerLoop->start;
    auto currentHalfEdge = startHalfEdge;
    do
    {
      auto curve = currentHalfEdge->pCurve.get();
      auto curveDomain = curve->getDomain();
      for (size_t x = 0; x <= CURVE_SAMPLING_RESOLUTION; ++x)
      {
        auto tx = static_cast<float>(x) / CURVE_SAMPLING_RESOLUTION;
        auto t = tFromDomain(tx, curveDomain.min, curveDomain.max);
        auto uv = curve->getPoint(t);
        faceDomain.uMin = std::min(faceDomain.uMin, uv.x);
        faceDomain.uMax = std::max(faceDomain.uMax, uv.x);
        faceDomain.vMin = std::min(faceDomain.vMin, uv.y);
        faceDomain.vMax = std::max(faceDomain.vMax, uv.y);
      }
      currentHalfEdge = currentHalfEdge->next;
    } while (currentHalfEdge != startHalfEdge);

    return faceDomain;
  }

  bool BRepFace::isPointInside(const glm::vec2& uv) const
  {
    auto startHalfEdge = outerLoop->start;
    auto circle = dynamic_cast<CircleCurve2D*>(startHalfEdge->pCurve.get());

    if (!circle)
    {
      return true;
    }

    return glm::length(uv) <= circle->getRadius();
  }
}
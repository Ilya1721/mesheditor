#include "BRepUVMapper.h"

namespace MeshCore
{
  std::pair<float, float> AffineFaceUVMapper::faceToSurfaceUV(
    const SurfaceDomain& faceDomain, const SurfaceDomain& surfaceDomain, float u, float v
  ) const
  {
    auto su = tFaceToSurfaceDomain(
      u, faceDomain.uMin, faceDomain.uMax, surfaceDomain.uMin, surfaceDomain.uMax
    );
    auto sv = tFaceToSurfaceDomain(
      v, faceDomain.vMin, faceDomain.vMax, surfaceDomain.vMin, surfaceDomain.vMax
    );

    return std::make_pair(su, sv);
  }
}

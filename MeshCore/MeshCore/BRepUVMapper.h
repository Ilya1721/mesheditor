#pragma once

#include <utility>

#include "BRepDomain.h"

namespace MeshCore
{
  class BRepFaceUVMapper
  {
   public:
    virtual ~BRepFaceUVMapper() = default;
    virtual std::pair<float, float> faceToSurfaceUV(
      const SurfaceDomain& faceDomain,
      const SurfaceDomain& surfaceDomain,
      float u,
      float v
    ) const = 0;
  };

  class AffineFaceUVMapper : public BRepFaceUVMapper
  {
   public:
    std::pair<float, float> faceToSurfaceUV(
      const SurfaceDomain& face,
      const SurfaceDomain& surfaceDomain,
      float u,
      float v
    ) const override;
  };
}

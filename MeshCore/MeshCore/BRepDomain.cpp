#include "BRepDomain.h"

namespace MeshCore
{
  float tFromDomain(float t, float min, float max)
  {
    return min + t * (max - min);
  }

  float tFaceToSurfaceDomain(
    float t, float faceMin, float faceMax, float surfaceMin, float surfaceMax
  )
  {
    return surfaceMin + (t - faceMin) * (surfaceMax - surfaceMin) / (faceMax - faceMin);
  }
}
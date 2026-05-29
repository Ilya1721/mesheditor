#pragma once

namespace MeshCore
{
  struct CurveDomain
  {
    float min;
    float max;
  };

  struct SurfaceDomain
  {
    float uMin;
    float uMax;
    float vMin;
    float vMax;
  };

  float tFromDomain(float t, float min, float max);
  float tFaceToSurfaceDomain(
    float t, float faceMin, float faceMax, float surfaceMin, float surfaceMax
  );
}

#pragma once

namespace MeshCore
{
  constexpr bool SMOOTHING_ENABLED = false;
  constexpr float ARROW_HEAD_ANGLE = 30.0f;
  constexpr float ARROW_HEAD_LENGTH_COEF = 0.15f;
  constexpr float EPSILON = 1e-6f;
  constexpr size_t CURVE_SAMPLING_RESOLUTION = 16;
  constexpr int OCTREE_MAX_POINTS = 16;
  constexpr int OCTREE_MAX_DEPTH = 10;
  constexpr int OCTREE_SUBDIVISION = 8;
  constexpr int CUBE_CORNERS = 8;
  constexpr float MC_ISO = 0.5f;
  constexpr int VOXEL_RESOLUTION = 64;
}  // namespace MeshCore

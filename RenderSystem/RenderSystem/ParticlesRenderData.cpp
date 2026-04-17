#include "ParticlesRenderData.h"

namespace RenderSystem
{
  void ParticlesRenderData::append(const Particle& particle)
  {
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      mCompactData.push_back(particle.position[coordIdx]);
    }
    mCompactData.push_back(particle.sideLength);
    mCompactData.push_back(particle.viewDirRotation);
    mCompactData.push_back(particle.blendedFrameIdx);
    for (int coordIdx = 0; coordIdx < 4; ++coordIdx)
    {
      mCompactData.push_back(particle.colorMultiplier[coordIdx]);
    }
  }
}

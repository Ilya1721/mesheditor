#pragma once

#include "Particle.h"
#include "RenderData.h"

namespace RenderSystem
{
  class ParticlesRenderData : public RenderData
  {
   public:
    void append(const Particle& particle);
  };
}

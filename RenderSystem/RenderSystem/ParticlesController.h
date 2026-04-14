#pragma once

#include <vector>

#include "ImageTexture.h"
#include "Particle.h"

namespace RenderSystem
{
  class ParticlesController
  {
   public:
     void loadFlipbookTexture(const std::string& texturePath);

   private:
    std::vector<Particle> mParticles;
    ImageTexture mFlipbookTexture;
  };
}

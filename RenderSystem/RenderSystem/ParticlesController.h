#pragma once

#include <memory>
#include <random>
#include <unordered_set>
#include <vector>

#include "FlipbookTexture.h"
#include "Particle.h"

namespace RenderSystem
{
  struct Flipbook
  {
    std::unique_ptr<FlipbookTexture> texture;
    int rows {};
    int cols {};
    int totalFrames {};
  };

  class ParticlesController
  {
   public:
    ParticlesController();

    const Flipbook& getFlipbook() const;
    const std::vector<Particle>& getParticles() const;
    const std::unordered_set<size_t>& getActiveParticlesIndices() const;

    void update(float lastFrameTime);
    void startGeneratingParticles(const glm::vec3& point);
    void stopGeneratingParticles();
    void moreParticles();
    void lessParticles();

   private:
    float getIncreaseStep() const;
    float randomFloat(float min, float max) const;
    size_t getFirstInactiveParticleIdx() const;
    void activateParticles(int particlesToGenerate);
    void updateActiveParticles(float lastFrameTime);
    void updateActiveParticle(size_t particleIdx, float lastFrameTime);
    void activateParticle(size_t particleIdx);
    void deactivateParticle(size_t particleIdx);
    void init();
    void reset();

   private:
    Flipbook mFlipbook;
    std::vector<Particle> mParticles;
    std::unordered_set<size_t> mActiveParticlesIndices;
    Emitter mEmitter;
    float mParticlesPerSecond;
    float mParticlesRemainder;
    bool mGenerateParticles;
    mutable std::mt19937 mRandomNumberGenerator;
  };
}

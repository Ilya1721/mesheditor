#include "ParticlesController.h"

#include <glm/gtc/constants.hpp>

#include "Constants.h"

namespace RenderSystem
{
  ParticlesController::ParticlesController()
    : mParticlesPerSecond(PARTICLES_PER_SECOND),
      mParticlesRemainder(),
      mGenerateParticles(false),
      mRandomNumberGenerator(std::random_device {}())
  {
    init();
  }

  const Flipbook& ParticlesController::getFlipbook() const
  {
    return mFlipbook;
  }

  const std::vector<Particle>& ParticlesController::getParticles() const
  {
    return mParticles;
  }

  const std::unordered_set<size_t>& ParticlesController::getActiveParticlesIndices() const
  {
    return mActiveParticlesIndices;
  }

  void ParticlesController::init()
  {
    mFlipbook.texture = std::make_unique<FlipbookTexture>(FIRE_FLIPBOOK_PATH);
    mFlipbook.rows = 6;
    mFlipbook.cols = 6;
    mFlipbook.totalFrames = mFlipbook.rows * mFlipbook.cols;
    mParticles.resize(MAX_PARTICLES);
  }

  void ParticlesController::reset()
  {
    mParticles.clear();
    mParticles.resize(MAX_PARTICLES);
    mActiveParticlesIndices.clear();
    mParticlesPerSecond = PARTICLES_PER_SECOND;
    mParticlesRemainder = 0.0f;
  }

  void ParticlesController::update(float lastFrameTime)
  {
    if (!mGenerateParticles)
    {
      return;
    }

    mParticlesRemainder += mParticlesPerSecond * lastFrameTime;
    auto particlesToGenerate = static_cast<int>(mParticlesRemainder);
    mParticlesRemainder -= particlesToGenerate;

    updateActiveParticles(lastFrameTime);
    activateParticles(particlesToGenerate);
  }

  void ParticlesController::startGeneratingParticles(const glm::vec3& point)
  {
    reset();
    mEmitter.position = point;
    mGenerateParticles = true;
  }

  void ParticlesController::stopGeneratingParticles()
  {
    reset();
    mGenerateParticles = false;
  }

  void ParticlesController::moreParticles()
  {
    auto particlesPerSecond = mParticlesPerSecond + getIncreaseStep();
    if (static_cast<size_t>(particlesPerSecond) <= MAX_PARTICLES)
    {
      mParticlesPerSecond = particlesPerSecond;
    }
  }

  void ParticlesController::lessParticles()
  {
    auto particlesPerSecond = mParticlesPerSecond - getIncreaseStep();
    if (static_cast<size_t>(particlesPerSecond) >= MIN_PARTICLES)
    {
      mParticlesPerSecond = particlesPerSecond;
    }
  }

  float ParticlesController::getIncreaseStep() const
  {
    return mParticlesPerSecond * 0.1;
  }

  float ParticlesController::randomFloat(float min, float max) const
  {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(mRandomNumberGenerator);
  }

  size_t ParticlesController::getFirstInactiveParticleIdx() const
  {
    for (size_t particleIdx = 0; particleIdx < mParticles.size(); ++particleIdx)
    {
      if (!mParticles[particleIdx].isActive)
      {
        return particleIdx;
      }
    }

    return 0;
  }

  void ParticlesController::activateParticles(int particlesToGenerate)
  {
    for (int counter = 0; counter <= particlesToGenerate; ++counter)
    {
      auto particleIdx = getFirstInactiveParticleIdx();
      activateParticle(particleIdx);
    }
  }

  void ParticlesController::updateActiveParticles(float lastFrameTime)
  {
    for (size_t particleIdx = 0; particleIdx < mParticles.size(); ++particleIdx)
    {
      updateActiveParticle(particleIdx, lastFrameTime);
    }
  }

  void ParticlesController::updateActiveParticle(size_t particleIdx, float lastFrameTime)
  {
    auto& particle = mParticles[particleIdx];
    if (!particle.isActive)
    {
      return;
    }

    particle.currentTime += lastFrameTime;
    if (particle.currentTime >= particle.duration)
    {
      deactivateParticle(particleIdx);
      return;
    }

    auto lifeTime = particle.currentTime / particle.duration;
    particle.blendedFrameIdx = lifeTime * mFlipbook.totalFrames;
    particle.colorMultiplier.a = 1.0f - lifeTime;
    particle.sideLength = glm::mix(0.2f, 1.0f, glm::sin(lifeTime * glm::pi<float>()));
    particle.position += particle.velocity * lastFrameTime;
  }

  void ParticlesController::activateParticle(size_t particleIdx)
  {
    auto& particle = mParticles[particleIdx];
    particle.isActive = true;
    particle.position = mEmitter.position;
    particle.velocity = glm::vec3(
      randomFloat(-0.5f, 0.5f), randomFloat(1.0f, 3.0f), randomFloat(-0.5f, 0.5f)
    );
    particle.sideLength = randomFloat(0.2f, 0.8f);
    particle.viewDirRotation = randomFloat(0.0f, glm::two_pi<float>());
    particle.currentTime = 0.0f;
    particle.duration = randomFloat(0.8f, 1.5f);
    particle.blendedFrameIdx = randomFloat(0.0f, mFlipbook.totalFrames);
    particle.colorMultiplier = glm::vec4(1.0f);
    mActiveParticlesIndices.insert(particleIdx);
  }

  void ParticlesController::deactivateParticle(size_t particleIdx)
  {
    auto& particle = mParticles[particleIdx];
    particle.isActive = false;
    mActiveParticlesIndices.erase(particleIdx);
  }
}

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
    mGenerateParticles = false;
  }

  void ParticlesController::moreParticles()
  {
    mParticlesPerSecond += getIncreaseStep();
  }

  void ParticlesController::lessParticles()
  {
    mParticlesPerSecond -= getIncreaseStep();
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
    }
  }

  void ParticlesController::updateActiveParticles(float lastFrameTime)
  {
    for (auto& particle : mParticles)
    {
      if (!particle.isActive)
      {
        continue;
      }

      particle.currentTime += lastFrameTime;
      if (particle.currentTime >= particle.duration)
      {
        particle.isActive = false;
        continue;
      }

      auto lifeTime = particle.currentTime / particle.duration;
      particle.blendedFrameIdx = lifeTime * mFlipbook.totalFrames;
      particle.colorMultiplier.a = 1.0f - lifeTime;
      particle.sideLength = glm::mix(0.2f, 1.0f, glm::sin(lifeTime * glm::pi<float>()));
      particle.position += particle.velocity * lastFrameTime;
    }
  }
}

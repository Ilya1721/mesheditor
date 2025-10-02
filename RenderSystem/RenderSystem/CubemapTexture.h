#pragma once

#include <array>
#include <filesystem>

#include "Texture.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class CubemapTexture : public Texture
  {
   public:
    CubemapTexture(const std::array<path, 6>& cubemapFaces);

    void passToFragmentShader(int textureLocation) const override;

   private:
    void initParameters();
    void loadCubemapFaces(const std::array<path, 6>& faces);
    void bind() const override;
    void unbind() const override;
  };
}  // namespace RenderSystem

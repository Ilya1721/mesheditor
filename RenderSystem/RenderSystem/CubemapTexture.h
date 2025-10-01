#pragma once

#include <array>
#include <filesystem>

using namespace std::filesystem;

namespace RenderSystem
{
  class CubemapTexture
  {
   public:
    CubemapTexture(const std::array<path, 6>& cubemapFaces);
    ~CubemapTexture();

   private:
    void initParameters();
    void loadCubemapFaces(const std::array<path, 6>& faces);

   private:
    unsigned int mTexture;
  };
}  // namespace RenderSystem

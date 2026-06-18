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

    unsigned int getId() const;
    void bind() const;

   private:
    void initParameters() const;
    void loadCubemapFaces(const std::array<path, 6>& faces);
    void createCubemapTexture(
      int width, int height, const unsigned char* data, int colorChannels, int faceIdx
    ) const;

  private:
    unsigned int mTexture;
  };
}  // namespace RenderSystem

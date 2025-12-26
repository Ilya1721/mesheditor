#pragma once

#include <filesystem>

#include "Object3D.h"

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadOBJModel(const std::filesystem::path& filePath);
}  // namespace RenderSystem

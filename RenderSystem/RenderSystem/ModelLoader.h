#pragma once

#include <filesystem>

#include "Object3D.h"

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadModel(const std::filesystem::path& path);
  std::vector<Vertex> loadVertices(const std::filesystem::path& path);
}

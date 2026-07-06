#pragma once

#include <filesystem>
#include <memory>

#include "ModelLoaderUtils.h"
#include "Object3D.h"

namespace RenderSystem
{
  std::unique_ptr<Object3D> loadModel(
    const std::filesystem::path& path, const ModelLoaderConfig& config = {}
  );
}

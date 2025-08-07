#pragma once

#include <filesystem>
#include <functional>

#include "RequiresBindBeforeInvoke.h"

namespace RenderSystem
{
  class ShaderProgram : public RequiresBindBeforeInvoke
  {
   public:
    ShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

   protected:
    virtual ~ShaderProgram();

   private:
    void init();
    void initShaderProgram();
    void bind() override;
    void unbind() override;

   protected:
    std::filesystem::path mVertexShaderPath;
    std::filesystem::path mFragmentShaderPath;

    int mVertexShader;
    int mFragmentShader;
    int mShaderProgram;
  };
}  // namespace RenderSystem

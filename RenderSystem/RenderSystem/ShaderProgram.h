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

    int getUniformLocation(const char* name) const;

   private:
    void init();
    void initShaderProgram();
    void bind() const override;
    void unbind() const override;

   protected:
    std::filesystem::path mVertexShaderPath;
    std::filesystem::path mFragmentShaderPath;

    int mVertexShader;
    int mFragmentShader;
    int mShaderProgram;
  };
}  // namespace RenderSystem

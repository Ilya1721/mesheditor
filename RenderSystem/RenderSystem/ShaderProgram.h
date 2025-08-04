#pragma once

#include <filesystem>
#include <functional>

namespace RenderSystem
{
  class ShaderProgram
  {
   public:
    ShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void invokeAction(const std::function<void()>& action) const;

   protected:
    virtual ~ShaderProgram();

   private:
    void init();
    void initShaderProgram();

   protected:
    std::filesystem::path mVertexShaderPath;
    std::filesystem::path mFragmentShaderPath;

    int mVertexShader;
    int mFragmentShader;
    int mShaderProgram;
  };
}  // namespace RenderSystem

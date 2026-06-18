#pragma once

#include <filesystem>
#include <functional>
#include <glm/glm.hpp>

namespace RenderSystem
{
  struct Material;

  class ShaderProgram
  {
   public:
    ShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );
    virtual ~ShaderProgram();
    virtual void preRenderSetup() const {};

    void bind() const;

   protected:
    int getUniformLocation(const char* name) const;
    bool getFlagFromShader(int flagLocation) const;

   private:
    void init(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void initShaderProgram();

   protected:
    int mVertexShader;
    int mFragmentShader;
    int mShaderProgram;
  };

  class Object3DShaderProgram : public ShaderProgram
  {
   public:
    Object3DShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );
    virtual void setModel(const glm::mat4& model) const = 0;
    virtual void setMaterial(const Material& material) const = 0;
  };
}  // namespace RenderSystem

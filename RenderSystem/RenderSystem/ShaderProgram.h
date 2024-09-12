#pragma once

#include <string>

namespace RenderSystem
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        virtual ~ShaderProgram();

        void setModel(const float* model) const;
        void use() const;

    private:
        void init();
        void initShaderProgram();
        void initUniformLocations();

    protected:
        int mVertexShader;
        int mFragmentShader;
        int mShaderProgram;
        int mModel;

        std::string mVertexShaderPath;
        std::string mFragmentShaderPath;
    };
}

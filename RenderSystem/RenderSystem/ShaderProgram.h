#pragma once

#include <string>

namespace RenderSystem
{
    class ShaderProgram
    {
    public:
        ShaderProgram();
        virtual ~ShaderProgram();

        void setModel(const float* model) const;
        void use() const;

    protected:
        void init(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:
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

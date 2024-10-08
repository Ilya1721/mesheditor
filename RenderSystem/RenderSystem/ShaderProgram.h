#pragma once

#include <filesystem>

namespace RenderSystem
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);
        virtual ~ShaderProgram();

        void setModel(const float* model) const;
        void setView(const float* view) const;
        void setProjection(const float* projection) const;

    private:
        void init();
        void initShaderProgram();
        void initUniformLocations();

    protected:
        std::filesystem::path mVertexShaderPath;
        std::filesystem::path mFragmentShaderPath;

        int mVertexShader;
        int mFragmentShader;
        int mShaderProgram;

        int mModel;
        int mView;
        int mProjection;
    };
}

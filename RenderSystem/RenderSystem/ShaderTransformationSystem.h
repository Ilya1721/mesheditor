#pragma once

namespace RenderSystem
{
    class ShaderTransformationSystem
    {
    public:
        ShaderTransformationSystem() = default;

        void init(int shaderProgram);
        void setViewModel(const float* view);
        void setProjection(const float* projection);

    private:
        void initUniformLocations();

    private:
        int mShaderProgram;
        int mViewModel;
        int mProjection;
    };
}
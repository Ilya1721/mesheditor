#pragma once

namespace RenderSystem
{
    class ShaderTransformationSystem
    {
    public:
        void init(int shaderProgram);
        void setViewModel(const float* view) const;
        void setProjection(const float* projection) const;

    private:
        void initUniformLocations();

    private:
        int mShaderProgram;
        int mViewModel;
        int mProjection;
    };
}
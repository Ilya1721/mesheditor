#pragma once

namespace RenderSystem
{
    class ShaderTransformationSystem
    {
    public:
        ShaderTransformationSystem() = default;

        void init(int shaderProgram);
        void setModel(const float* model);
        void setView(const float* view);
        void setProjection(const float* projection);

    private:
        void initUniformLocations();

    private:
        int mShaderProgram;
        int mModel;
        int mView;
        int mProjection;
    };
}
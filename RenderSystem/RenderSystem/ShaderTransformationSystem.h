#pragma once

namespace RenderSystem
{
    class ShaderTransformationSystem
    {
    public:
        void init(int shaderProgram);
        void setModel(const float* model) const;
        void setView(const float* view) const;
        void setProjection(const float* projection) const;

    private:
        void initUniformLocations();

    private:
        int mShaderProgram;
        int mModel;
        int mView;
        int mProjection;
    };
}
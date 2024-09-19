#include "SceneShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
    SceneShaderProgram::SceneShaderProgram() :
        ShaderProgram(),
		mProjection(),
		mView(),
		lighting()
    {}

	void SceneShaderProgram::init(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		ShaderProgram::init(vertexShaderPath, fragmentShaderPath);
		initUniformLocations();
		lighting.init(mShaderProgram);
	}

	void SceneShaderProgram::initUniformLocations()
	{
		mView = glGetUniformLocation(mShaderProgram, "view");
		mProjection = glGetUniformLocation(mShaderProgram, "projection");
	}

	void SceneShaderProgram::setView(const float* view) const
	{
		glUniformMatrix4fv(mView, 1, false, view);
	}

	void SceneShaderProgram::setProjection(const float* projection) const
	{
		glUniformMatrix4fv(mProjection, 1, false, projection);
	}
}
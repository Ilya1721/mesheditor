#include "DepthShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
    DepthShaderProgram::DepthShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :
        ShaderProgram(vertexShaderPath, fragmentShaderPath),
        mLightSpaceTransform()
    {
        init();
    }

	void DepthShaderProgram::setLightSpaceTransform(const float* transform) const
	{
		glUniformMatrix4fv(mLightSpaceTransform, 1, false, transform);
	}

	void DepthShaderProgram::init()
	{
		initUniformLocations();
	}

	void DepthShaderProgram::initUniformLocations()
	{
		mLightSpaceTransform = glGetUniformLocation(mShaderProgram, "lightSpaceTransform");
	}
}
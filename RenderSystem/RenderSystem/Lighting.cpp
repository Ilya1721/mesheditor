#include "Lighting.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "Constants.h"

namespace
{
	using namespace RenderSystem;

	void setupSettings()
	{
		glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
		glEnable(GL_DEPTH_TEST);
	}
}

namespace RenderSystem
{
	void Lighting::init(int shaderProgram)
	{
		mShaderProgram = shaderProgram;
		initUniformLocations();
		setupSettings();
	}

	int Lighting::getUniformLocation(const char* name) const
	{
		return glGetUniformLocation(mShaderProgram, name);
	}

	void Lighting::initUniformLocations()
	{
		mLightPos = getUniformLocation("lightPos");
		mCameraPos = getUniformLocation("cameraPos");
		mObjectColor = getUniformLocation("objectColor");
		mLightColor = getUniformLocation("lightColor");
		mAmbientStrength = getUniformLocation("ambientStrength");
		mSpecularStrength = getUniformLocation("specularStrength");
		mShininess = getUniformLocation("shininess");
	}

	void Lighting::setLightPos(const float* lightPos)
	{
		glUniform3fv(mLightPos, 1, lightPos);
	}

	void Lighting::setObjectColor(const float* color)
	{
		glUniform3fv(mObjectColor, 1, color);
	}

	void Lighting::setLightColor(const float* color)
	{
		glUniform3fv(mLightColor, 1, color);
	}

	void Lighting::setCameraPos(const float* pos)
	{
		glUniform3fv(mCameraPos, 1, pos);
	}

	void Lighting::setAmbientStrength(float strength)
	{
		glUniform1f(mAmbientStrength, strength);
	}

	void Lighting::setSpecularStrength(float strength)
	{
		glUniform1f(mSpecularStrength, strength);
	}

	void Lighting::setShininess(int shininess)
	{
		glUniform1i(mShininess, shininess);
	}
}

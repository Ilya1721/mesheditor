#include "Lighting.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"

namespace RenderSystem
{
	void Lighting::init(int shaderProgram)
	{
		mShaderProgram = shaderProgram;
		initUniformLocations();
		setupSettings();
	}

	void Lighting::setupSettings()
	{
		glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
		glEnable(GL_DEPTH_TEST);
		setLight(DEFAULT_LIGHT);
		setMaterial(GOLD_MATERIAL);
	}

	int Lighting::getUniformLocation(const char* name) const
	{
		return glGetUniformLocation(mShaderProgram, name);
	}

	void Lighting::initUniformLocations()
	{
		mLightPos = getUniformLocation("lightPos");
		mCameraPos = getUniformLocation("cameraPos");
		mLightAmbient = getUniformLocation("light.ambient");
		mLightDiffuse = getUniformLocation("light.diffuse");
		mLightSpecular = getUniformLocation("light.specular");
		mMaterialAmbient = getUniformLocation("material.ambient");
		mMaterialDiffuse = getUniformLocation("material.diffuse");
		mMaterialSpecular = getUniformLocation("material.specular");
		mMaterialShininess = getUniformLocation("material.shininess");
	}

	void Lighting::setMaterial(const Material& material)
	{
		setMaterialAmbient(glm::value_ptr(material.ambient));
		setMaterialDiffuse(glm::value_ptr(material.diffuse));
		setMaterialSpecular(glm::value_ptr(material.specular));
		setMaterialShininess(material.shininess);
	}

	void Lighting::setLight(const Light& light)
	{
		setLightAmbient(glm::value_ptr(light.ambient));
		setLightDiffuse(glm::value_ptr(light.diffuse));
		setLightSpecular(glm::value_ptr(light.specular));
	}

	void Lighting::setLightPos(const float* lightPos)
	{
		glUniform3fv(mLightPos, 1, lightPos);
	}

	void Lighting::setCameraPos(const float* pos)
	{
		glUniform3fv(mCameraPos, 1, pos);
	}

	void Lighting::setLightAmbient(const float* ambient)
	{
		glUniform3fv(mLightAmbient, 1, ambient);
	}

	void Lighting::setLightDiffuse(const float* diffuse)
	{
		glUniform3fv(mLightDiffuse, 1, diffuse);
	}

	void Lighting::setLightSpecular(const float* specular)
	{
		glUniform3fv(mLightSpecular, 1, specular);
	}

	void Lighting::setMaterialAmbient(const float* ambient)
	{
		glUniform3fv(mMaterialAmbient, 1, ambient);
	}

	void Lighting::setMaterialDiffuse(const float* diffuse)
	{
		glUniform3fv(mMaterialDiffuse, 1, diffuse);
	}

	void Lighting::setMaterialSpecular(const float* specular)
	{
		glUniform3fv(mMaterialSpecular, 1, specular);
	}

	void Lighting::setMaterialShininess(float shininess)
	{
		glUniform1f(mMaterialShininess, shininess);
	}
}

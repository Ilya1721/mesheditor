#pragma once

namespace RenderSystem
{
	class Lighting
	{
	public:
		Lighting() = default;

		void init(int shaderProgram);
		void setLightPos(const float* lightPos);
		void setCameraPos(const float* pos);
		void setLightAmbient(const float* ambient);
		void setLightDiffuse(const float* diffuse);
		void setLightSpecular(const float* specular);
		void setMaterialAmbient(const float* ambient);
		void setMaterialDiffuse(const float* diffuse);
		void setMaterialSpecular(const float* specular);
		void setMaterialShininess(float shininess);

	private:
		void initUniformLocations();
		int getUniformLocation(const char* name) const;
		void setupSettings();

	private:
		int mShaderProgram;
		int mLightPos;
		int mCameraPos;
		int mLightAmbient;
		int mLightDiffuse;
		int mLightSpecular;
		int mMaterialAmbient;
		int mMaterialDiffuse;
		int mMaterialSpecular;
		int mMaterialShininess;
	};
}


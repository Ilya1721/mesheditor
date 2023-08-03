#pragma once

namespace RenderSystem
{
	class Lighting final
	{
	public:
		Lighting();

		void init(int shaderProgram);

		void setLightPos(const float* lightPos);
		void setObjectColor(const float* color);
		void setLightColor(const float* color);
		void setCameraPos(const float* pos);
		void setAmbientStrength(float strength);
		void setSpecularStrength(float strength);
		void setShininess(int shininess);

	private:
		void initUniformLocations();
		void setUpSettings();

	private:
		int mShaderProgram;

		int mLightPos;
		int mObjectColor;
		int mLightColor;
		int mAmbientStrength;
		int mSpecularStrength;
		int mShininess;
		int mCameraPos;
	};
}


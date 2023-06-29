export module Lighting;

export namespace RenderSystem
{
	class Lighting final
	{
	public:
		Lighting();

		void init(int shaderProgram);

		void setLightPos(const float* lightPos);
		void setObjectColor(const float* color);
		void setAmbientColor(const float* color);
		void setDiffuseColor(const float* color);
		void setAmbientStrength(float strength);

	private:
		void initUniformLocations();
		void setUpSettings();

	private:
		int mShaderProgram;

		int mLightPos;
		int mObjectColor;
		int mAmbientColor;
		int mAmbientStrength;
		int mDiffuseColor;
	};
}


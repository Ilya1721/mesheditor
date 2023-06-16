export module BasicRenderer;

import std;

export namespace RenderSystem
{
	enum class SHADER_LOG_TYPE
	{
		SHADER,
		SHADER_PROGRAM
	};

	class BasicRenderer
	{
	public:
		BasicRenderer() noexcept;
		BasicRenderer(const BasicRenderer& renderer) = delete;
		BasicRenderer(BasicRenderer&& renderer) = delete;
		BasicRenderer& operator=(const BasicRenderer& renderer) = delete;
		BasicRenderer& operator=(BasicRenderer&& renderer) = delete;
		virtual ~BasicRenderer() noexcept;

		virtual void init() noexcept;

		virtual int getBasicVertexShader() const noexcept;
		virtual int getBasicFragmentShader() const noexcept;

		virtual void beginUse() noexcept;
		virtual void endUse() noexcept;

		void setMVP(const double* mvp) noexcept;

		static void printOpenGLErrorMessage() noexcept;

	protected:
		virtual int loadShader(const std::string& filePath, int shaderType) noexcept;
		virtual std::vector<char> getShaderLog(int shaderOrProgramId, SHADER_LOG_TYPE logType) noexcept;
	protected:

		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;
		int mMVP;
	};
}


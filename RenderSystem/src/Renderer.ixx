export module Renderer;

import <vector>;
import <string>;

import RenderData;

export namespace RenderSystem
{
	enum class SHADER_LOG_TYPE
	{
		SHADER,
		SHADER_PROGRAM
	};

	class Renderer final
	{
	public:
		Renderer();
		~Renderer() noexcept;

		void setRenderData(const MeshCore::RenderData& renderData) noexcept;
		void setModel(const float* model) noexcept;
		void setView(const float* view) noexcept;
		void setProjection(const float* projection) noexcept;

		void render() const noexcept;

		static void printOpenGLErrorMessage() noexcept;

	private:
		int loadShader(const std::string& filePath, int shaderType);
		std::string getLog(int shaderOrProgramId, SHADER_LOG_TYPE logType) noexcept;
		void init();
		void initShaders();

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		int mModel;
		int mView;
		int mProjection;

		unsigned int mVBO;
		unsigned int mVAO;
	};
}


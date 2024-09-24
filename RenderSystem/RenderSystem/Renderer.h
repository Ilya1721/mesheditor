#pragma once

#include <string>
#include <functional>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"

#include "Lighting.h"
#include "RenderBuffer.h"
#include "ShaderTransformationSystem.h"
#include "Constants.h"

using namespace GeometryCore;

namespace MeshCore
{
	class Object3D;
}

namespace RenderSystem
{
	struct RenderPrimitive;

	class Renderer
	{
	public:
		Renderer();
		Renderer(Renderer&&) = delete;
		~Renderer();

		void setModel(const float* model) const;
		void setView(const float* view) const;
		void setProjection(const float* projection) const;

		Lighting& getLighting();
		RenderBuffer& getRenderBuffer();

		void render();

	private:
		void init();
		void initShaders();
		void initShaderProgram();
		void renderExtra();
		void renderHighlightedFaces();
		void renderWireframe();
		void renderWholeObjectHighlighted();
		void renderScene();
		void renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);
		void registerCallbacks();

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mRenderBuffer;
		RenderBuffer mExtraRenderBuffer;
	};
}


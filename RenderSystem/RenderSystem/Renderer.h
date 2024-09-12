#pragma once

#include <string>
#include <functional>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"

#include "RenderBuffer.h"
#include "Constants.h"
#include "SceneShaderProgram.h"
#include "DepthShaderProgram.h"

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
		static Renderer& getInstance();

		RenderBuffer& getRenderBuffer();

		void init();
		void registerCallbacks();
		void render();

	private:
		Renderer();
		~Renderer();

		void initExtraFBO();
		void renderExtra();
		void renderShadows();
		void renderHighlightedFaces();
		void renderWireframe();
		void renderWholeObjectHighlighted();
		void renderScene();
		void renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);
		void renderExtraFBO(const std::function<void()>& renderFunc) const;
		void adjustLightPos();

	private:
		unsigned int mExtraFBO;
		RenderBuffer mRenderBuffer;
		RenderBuffer mExtraRenderBuffer;
		SceneShaderProgram mSceneShaderProgram;
		DepthShaderProgram mDepthShaderProgram;

		static Renderer sInstance;
	};
}

